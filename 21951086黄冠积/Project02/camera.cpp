#include "Camera.h"



Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
{
	Position = position;
	WorldUp = worldup;
	Forward = glm::normalize(target - position);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
	Position = position;
	WorldUp = worldup;
	Pitch = pitch;
	Yaw = yaw;
	Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Forward, WorldUp);
}


void Camera::UpdateCameraVectors()
{
	Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY)
{
	Pitch -= deltaY * senseY;
	Yaw -= deltaX * senseX;
	UpdateCameraVectors();
}

void Camera::UpdatePosition(char direction)
{
	if (direction == 'W')
	{
		Position += Forward * move_speed;
	}
	else if (direction == 'S')
	{
		Position -= Forward * move_speed;
	}
	else if (direction == 'A')
	{
		Position -= Right * move_speed;
	}
	else if (direction == 'D')
	{
		Position += Right * move_speed;
	}
	else if (direction == 'Q')
	{
		Position -= Up * move_speed;
	}
	else if (direction == 'E')
	{
		Position += Up * move_speed;
	}
}


Camera::~Camera()
{
}

