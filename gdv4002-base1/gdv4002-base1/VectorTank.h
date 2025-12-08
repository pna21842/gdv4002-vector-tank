#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject2D.h"


class VectorTank : public GameObject2D {

private:

	// Derived values - matrices based on position and orientation
	glm::mat4		T, R, TR; // T (translation); R (rotation)


	// Private API
	void calculateDerivedMatices(void);

public:

	VectorTank(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID);

	// Accessor methods
	const glm::mat4& getRotationMatrix(void);
	const glm::mat4& getModelTransformMatrix(void);

	void update(double tDelta) override;
	void render(void) override;
};
