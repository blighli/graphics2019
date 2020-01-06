#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target,glm::vec3 worldup);
	Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);

	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 WorldUp;

	float Pitch;
	float Yaw;

	float senseX = 0.001f;
	float senseY = 0.001f;

	float move_speed = 0.002f;

	glm::mat4 GetViewMatrix();

	void ProcessMouseMovement(float deltaX, float deltaY);
	void UpdatePosition(char direction);


	~Camera();

private:
	void UpdateCameraVectors();

};

