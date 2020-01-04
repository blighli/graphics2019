#include "Camera.h"


void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Pitch > 89.0f) Pitch = 89.0f;
	if (Pitch < -89.0f) Pitch = -89.0f;

	/*Front.x = cos(toRadians(Yaw)) * cos(toRadians(Pitch));
	Front.y = sin(toRadians(Pitch));
	Front.z = sin(toRadians(Yaw)) * cos(toRadians(Pitch));
	Front = vec3::normalize(Front);*/

}

void Camera::ProcessMouseMovementXY(float xoffset, float yoffset) {
	Position.x += -xoffset * Speed;
	Position.y += yoffset * Speed;
}

void Camera::ProcessMouseScroll(float _yoffset) {
	cout << Position << endl;
	cout << Front << endl;
	Position += _yoffset * Sensitivity * Front;
	cout << Position << endl << endl;
}



mat4 Camera::getViewMatrix() {
	mat4 view(1.0f);
	mat4 rotatPitch = mat4::rotation(Pitch, vec3(1.0f, 0.0f, 0.0f));
	mat4 rotatYaw = mat4::rotation(Yaw, vec3(0.0f, 1.0f, 0.0f));
	mat4 trans = mat4::translation(vec3(-Position.x, -Position.y, -Position.z));

	view = view * rotatPitch * rotatYaw * trans;
	
	return view;
}


/*mat4 lookAt(vec3 position, vec3 target, vec3 worldup) {
	vec3 zaxis = vec3::normalize(position - target);
	vec3 xaxis = vec3::normalize(vec3::cross(worldup, zaxis));
	vec3 yaxis = vec3::cross(zaxis, xaxis);

	//¹Û²ì¾ØÕóLookAt = rotation * translation
	mat4 rotation, translation;
	rotation.colunms[0] = { xaxis.x, yaxis.x, zaxis.x, 0 };
	rotation.colunms[1] = { xaxis.y, yaxis.y, zaxis.y, 0 };
	rotation.colunms[2] = { xaxis.z, yaxis.z, zaxis.z, 0 };
	rotation.colunms[3] = { 0, 0, 0, 1 };

	translation.colunms[0] = { 1, 0, 0, 0 };
	translation.colunms[1] = { 0, 1, 0, 0 };
	translation.colunms[2] = { 0, 0, 1, 0 };
	translation.colunms[3] = { -position.x, -position.y, -position.z, 1 };

	mat4 temp;
	temp = rotation * translation;
	return temp;
}*/