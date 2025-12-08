
#include "VectorTank.h"
#include "KeyDefs.h"
#include "Engine.h"
#include <bitset>

extern std::bitset<5> keys;

using namespace glm;


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

	// Pull out the x basis vector from the rotation matrix
	glm::vec4 xBasis = R[0];
	glm::vec2 forwardDirection = glm::vec2(xBasis.x, xBasis.y);

	if (keys[Key::W])
		position += forwardDirection * distancePerSecond * (float)tDelta;
	else if (keys[Key::S])
		position -= forwardDirection * distancePerSecond * (float)tDelta;

	if (keys[Key::A])
		orientation += rotationPerSecond * (float)tDelta;
	else if (keys[Key::D])
		orientation -= rotationPerSecond * (float)tDelta;

	calculateDerivedMatices();
}

void VectorTank::render(void) {

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