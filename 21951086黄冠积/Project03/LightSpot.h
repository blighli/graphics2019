#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class LightSpot
{
public:
	LightSpot(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f)):
		position(_position),
		angles(_angles),
		color(_color)
	{
		constant = 1.0f;
		linear = 0.09f;
		quadratic = 0.032f;
		UpdateDirection();
	}
	~LightSpot() {};

	glm::vec3 position;
	glm::vec3 angles;
	glm::vec3 direction = glm::vec3(0, 0, 1.0f);
	glm::vec3 color;

	void UpdateDirection()
	{
		direction = glm::vec3(0, 0, 1.0f);
		direction = glm::rotateZ(direction, angles.z);
		direction = glm::rotateX(direction, angles.x);
		direction = glm::rotateY(direction, angles.y);
		direction = -1.0f * direction;
	}

	float cosPhy = 0.9f;

	float cosInnerPhy = 0.9f;
	float cosOutterPhy = 0.85f;

	float constant;
	float linear;
	float quadratic;


};

