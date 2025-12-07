#include "Engine.h"
#include "glPrint.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <bitset>
#include "KeyDefs.h"

using namespace glm;

class VectorTank;

// Function prototypes
void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void myRender(GLFWwindow* window);
void displayTransformMatrix(VectorTank* tank);

// Global variables
std::bitset<5> keys{ 0 };


#pragma region Vector Tank declaration


class VectorTank : public GameObject2D {

private:
		
	// Tank velocity (scalar value - we'll use orientation to determine direction)
	// Distance per-second (constant for this demo)
	const float		velocity = 0.4f;

	// Derived values - matrices based on position and orientation
	glm::mat4		T, R, TR; // T (translation); R (rotation)


	// Private API
	void calculateDerivedMatices(void);

public:

	VectorTank(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID);

	// Accessor methods
	const glm::mat4& getRotationMatrix(void);
	const glm::mat4& getModelTransformMatrix(void);

	// Interaction methods

	// Move forward (direction = 1) or backward (direction = -1)
	void move(float tDelta, float direction);

	// Rotate by constant rate (30 degrees per second) in direction determined by direction = 1 (counter-clockwise - following convention) or -1 (clockwise)
	void rotate(float tDelta, float direction);

	void update(double tDelta) override;
	void render(void) override;
};

#pragma endregion


#pragma region Vector Tank definition

void VectorTank::calculateDerivedMatices(void) {

	const glm::mat4 I = glm::mat4(1.0f); // create identity matrix

	T = glm::translate(I, glm::vec3(position, 0.0f));
	R = glm::rotate(I, orientation, glm::vec3(0.0f, 0.0f, 1.0f));
	TR = T * R;
}

VectorTank::VectorTank(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID) : GameObject2D(initPosition, initOrientation, initSize, initTextureID) {

	calculateDerivedMatices();
}

const glm::mat4& VectorTank::getRotationMatrix(void) {

	return R;
}

const glm::mat4& VectorTank::getModelTransformMatrix(void) {

	return TR;
}

void VectorTank::update(double tDelta) {

	const float distancePerSecond = 0.5f;
	const float rotationPerSecond = glm::radians(45.0f);

	if (keys[Key::W])
		position += vec2(
			cosf(orientation) * distancePerSecond * tDelta,
			sinf(orientation) * distancePerSecond * tDelta);
	else if (keys[Key::S])
		position -= vec2(
			cosf(orientation) * distancePerSecond * tDelta,
			sinf(orientation) * distancePerSecond * tDelta);

	if (keys[Key::A])
		orientation += rotationPerSecond * (float)tDelta;
	else if (keys[Key::D])
		orientation -= rotationPerSecond * (float)tDelta;

	calculateDerivedMatices();
}

void VectorTank::render(void) {

	float w = getViewplaneWidth();
	float h = getViewplaneHeight();

	// Render principles axes
	glLineWidth(2.0f);
	glLineStipple(1, 0xF0F0);
	glEnable(GL_LINE_STIPPLE);

	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(w / 2.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, h / 2.0f);

	glEnd();

	glLineWidth(1.0f);
	glDisable(GL_LINE_STIPPLE);

	// Render tank

	glPushMatrix();

	glTranslatef(position.x, position.y, 0.0f);
	glRotatef(glm::degrees(orientation), 0.0f, 0.0f, 1.0f);
	glScalef(size.x, size.y, 1.0f);

	glColor3f(0.0f, 0.8f, 0.0f);

	// Tank body
	glBegin(GL_LINE_LOOP);

	glVertex2f(-0.1f, 0.05f);
	glVertex2f(0.1f, 0.05f);
	glVertex2f(0.1f, -0.05f);
	glVertex2f(-0.1f, -0.05f);

	glEnd();

	// Turret
	glBegin(GL_LINE_LOOP);

	glVertex2f(-0.05f, 0.04f);
	glVertex2f(0.075f, 0.0f);
	glVertex2f(-0.05f, -0.04f);

	glEnd();

	// principle axes
	
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.4f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, 0.4f);

	glEnd();

	glPopMatrix();
}

#pragma endregion


int main(void) {
	
	// Initialise the engine (create window, setup OpenGL backend)
	int initResult = engineInit("GDV4002 - Applied Maths for Games", 1024, 1024);

	// If the engine initialisation failed report error and exit
	if (initResult != 0) {

		printf("Cannot setup game window!!!\n");
		return initResult; // exit if setup failed
	}

	glBuildFont();

	//
	// Setup game scene objects here
	//
	VectorTank* tank = new VectorTank(glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f), 0);
	addObject("tank", tank);

	setKeyboardHandler(myKeyboardHandler);
	setRenderFunction(myRender);

	setBackgroundColour(glm::vec4(0.3f, 0.3f, 0.3f, 0.0f));

	hideAxisLines();

	// Enter main loop - this handles update and render calls
	engineMainLoop();

	// When we quit (close window for example), clean up engine resources
	engineShutdown();

	// return success :)
	return 0;
}

void myRender(GLFWwindow* window) {

	VectorTank* tank = dynamic_cast<VectorTank*>(getObject("tank"));
	tank->render();

	displayTransformMatrix(tank);
}



// Function to display text overlay showing the contents of the tank's transformation matrix
void displayTransformMatrix(VectorTank* tank) {

	// 2D overlay to display relevant information
	glMatrixMode(GL_PROJECTION);

	// Save current projection
	glPushMatrix();

	// Clear any projections that are present and set a 2D projection
	glLoadIdentity();
	glOrtho(-4.0f, 4.0f, -4.0f, 4.0f, -1, 1);


	// Print current angle...
	glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos2f(-4.0f, 2.0f);
	glPrint("Tank Angle = % .3f", glm::degrees(tank->orientation));

	glm::mat4 M = tank->getModelTransformMatrix();

	
	// Print x column vector (in red)
	glColor3f(1.0, 0.0, 0.0);

	glRasterPos2f(-4.0f, 3.6f);
	glPrint("% .3f", M[0].x);

	glRasterPos2f(-4.0f, 3.3f);
	glPrint("% .3f", M[0].y);

	glRasterPos2f(-4.0f, 3.0f);
	glPrint("% .3f", M[0].z);

	glRasterPos2f(-4.0f, 2.7f);
	glPrint("% .3f", M[0].w);

	
	// Print y column vector (in green)
	glColor3f(0.0, 1.0, 0.0);

	glRasterPos2f(-3.2f, 3.6f);
	glPrint("% .3f", M[1].x);

	glRasterPos2f(-3.2f, 3.3f);
	glPrint("% .3f", M[1].y);

	glRasterPos2f(-3.2f, 3.0f);
	glPrint("% .3f", M[1].z);

	glRasterPos2f(-3.2f, 2.7f);
	glPrint("% .3f", M[1].w);

	
	// Print z column vector (in blue)
	glColor3f(0.0f, 0.0f, 0.5f);

	glRasterPos2f(-2.4f, 3.6f);
	glPrint("% .3f", M[2].x);

	glRasterPos2f(-2.4f, 3.3f);
	glPrint("% .3f", M[2].y);

	glRasterPos2f(-2.4f, 3.0f);
	glPrint("% .3f", M[2].z);

	glRasterPos2f(-2.4f, 2.7f);
	glPrint("% .3f", M[2].w);


	// Print w column vector (in yellow)
	glColor3f(0.75f, 0.75f, 0.0f);

	glRasterPos2f(-1.6f, 3.6f);
	glPrint("% .3f", M[3].x);

	glRasterPos2f(-1.6f, 3.3f);
	glPrint("% .3f", M[3].y);

	glRasterPos2f(-1.6f, 3.0f);
	glPrint("% .3f", M[3].z);

	glRasterPos2f(-1.6f, 2.7f);
	glPrint("% .3f", M[3].w);
	

	// Return to previous projection transformation
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}


void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check if a key is pressed
	if (action == GLFW_PRESS) {

		// check which key was pressed...
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// If escape is pressed tell GLFW we want to close the window (and quit)
			glfwSetWindowShouldClose(window, true);
			break;
		case GLFW_KEY_W:
			keys[Key::W] = true;
			break;
		case GLFW_KEY_A:
			keys[Key::A] = true;
			break;
		case GLFW_KEY_S:
			keys[Key::S] = true;
			break;
		case GLFW_KEY_D:
			keys[Key::D] = true;
			break;

		case GLFW_KEY_SPACE:

			VectorTank* tank = (VectorTank*)getObject("tank");

			glm::mat4 M = tank->getRotationMatrix();

			printf("%.3f, %.3f, %.3f, %.3f\n", M[0].x, M[0].y, M[0].z, M[0].w);
			printf("%.3f, %.3f, %.3f, %.3f\n", M[1].x, M[1].y, M[1].z, M[1].w);
			printf("%.3f, %.3f, %.3f, %.3f\n", M[2].x, M[2].y, M[2].z, M[2].w);
			printf("%.3f, %.3f, %.3f, %.3f\n", M[3].x, M[3].y, M[3].z, M[3].w);

			printf("\n");
			break;
		}
	}
	// If not check a key has been released
	else if (action == GLFW_RELEASE) {

		// handle key release events
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// If escape is pressed tell GLFW we want to close the window (and quit)
			glfwSetWindowShouldClose(window, true);
			break;
		case GLFW_KEY_W:
			keys[Key::W] = false;
			break;
		case GLFW_KEY_A:
			keys[Key::A] = false;
			break;
		case GLFW_KEY_S:
			keys[Key::S] = false;
			break;
		case GLFW_KEY_D:
			keys[Key::D] = false;
			break;
		}
	}
}