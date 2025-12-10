#include "Engine.h"
#include "glPrint.h"
#include <bitset>
#include "KeyDefs.h"
#include "VectorTank.h"

// Global variables
std::bitset<5> keys{ 0 };
GLuint myFontNormal = 0;
GLuint myFontUnderline = 0;

// Function prototypes
void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void myRender(GLFWwindow* window);
void renderGlobalAxes();
void displayTransformMatrix(VectorTank* tank);


int main(void) {
	
	// Initialise the engine (create window, setup OpenGL backend)
	int initResult = engineInit("GDV4002 - Applied Maths for Games", 1024, 1024);

	// If the engine initialisation failed report error and exit
	if (initResult != 0) {

		printf("Cannot setup game window!!!\n");
		return initResult; // exit if setup failed
	}

	myFontNormal = glBuildFont(L"Consolas", 24);
	myFontUnderline = glBuildFont(L"Consolas", 24,  GLFONT_STYLE::BOLD | GLFONT_STYLE::UNDERLINE);


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

	// Render principles axes
	renderGlobalAxes();
	
	// Render tank
	VectorTank* tank = dynamic_cast<VectorTank*>(getObject("tank"));
	tank->render();

	displayTransformMatrix(tank);
}

void renderGlobalAxes() {

	float w = getViewplaneWidth();
	float h = getViewplaneHeight();

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
	glSetCurrentFont(myFontUnderline);

	glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos2f(-4.0f, 2.0f);
	glPrint("Tank Angle = % .3f", glm::degrees(tank->orientation));

	glm::mat4 M = tank->getModelTransformMatrix();

	
	// Print x column vector (in red)
	glSetCurrentFont(myFontNormal);

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
		}
	}
	// If not check a key has been released
	else if (action == GLFW_RELEASE) {

		// handle key release events
		switch (key)
		{
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