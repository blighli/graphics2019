#pragma once
#include "Vector.h"

mat4 lookAt(vec3 position, vec3 target, vec3 worldup);

class Camera {
public:
	vec3 Position;
	vec3 Front;

	float Yaw;
	float Pitch;

	float Sensitivity;
	float Speed;
	
	Camera(vec3 position = vec3(0.0f, 0.0f, 3.0f), vec3 front = vec3(0.0f, 0.0f, -1.0f), float yaw = 0.0f, float pitch = 0.0f, float sensitivity = 0.2f, float speed = 0.003f){
		Position = position;
		Front = front;
		Yaw = yaw;
		Pitch = pitch;
		Sensitivity = sensitivity;
		Speed = speed;
	}
	mat4 getViewMatrix();
	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessMouseMovementXY(float xoffset, float yoffset);
	void ProcessMouseScroll(float _yoffset);
};
