#include "window.h"
#include "Vector.h"
#include "shader.h"
#include "vertexArray.h"
#include "indexBuffer.h"
#include "Model.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float WINDOWS_WIDTH = 1280.0f, WINDOWS_HEIGHT = 720.0f;
float deltaTime = 0, lastFrame = 0;
float lastX = WINDOWS_WIDTH / 2.0f, lastY = WINDOWS_HEIGHT / 2.0f;
Camera camera;

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
int main() {

	Window window = Window("Game", WINDOWS_WIDTH, WINDOWS_HEIGHT);

	glfwSetScrollCallback(window.getWindow(), scroll_callback);

	Model sunModel("source/model/sun.obj");
	Model moonModel("source/model/moon.obj");
	Model earthModel("source/model/earth.obj");

	//纹理
	//sun
	unsigned int texture1, texture2, texture3;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, channels;
	unsigned char *data;
	data = stbi_load("source/model/sun.jpg", &width, &height, &channels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "load texture failed" << endl;
	}
	stbi_image_free(data);

	//moon
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("source/model/moon.jpg", &width, &height, &channels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "load texture failed" << endl;
	}
	stbi_image_free(data);

	//earth
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("source/model/earth.jpg", &width, &height, &channels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "load texture failed" << endl;
	}
	stbi_image_free(data);

	Shader shader("source/shaders/basic.vert", "source/shaders/basic.frag");
	Shader shader2("source/shaders/light.vert", "source/shaders/light.frag");

	//光照
	vec3 lightPos(1.2f, 1.0f, 2.0f);

	glEnable(GL_DEPTH_TEST);
	while (!window.closed()) {
		window.clear();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		//处理鼠标事件
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		double xpos, ypos;

		window.getCursorPosition(xpos, ypos);
		float xoffset = xpos - lastX;
		float yoffset = ypos - lastY;

		lastX = xpos;
		lastY = ypos;

		if (window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
			camera.ProcessMouseMovement(xoffset, yoffset);
		}
		if (window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
			camera.ProcessMouseMovementXY(xoffset, yoffset);
		}

		mat4 projection = mat4::perspective(45.0f, (float)WINDOWS_WIDTH / (float)WINDOWS_HEIGHT, 0.1f, 100.0f);
		mat4 model = mat4::scale(vec3(0.08, 0.08, 0.08)) * mat4::translation(vec3(0, -0.5, 0));
		mat4 view = camera.getViewMatrix();

		shader2.enable();
		shader2.setUniformMat4("projection_matrix", projection);
		shader2.setUniformMat4("model_matrix", model);
		shader2.setUniformMat4("view_matrix", view);
		shader2.setUniform3f("lightColor", vec3(1.0f, 1.0f, 1.0f));
		shader2.setUniform3f("lightPos", lightPos);
		shader2.setUniform3f("objectColor", vec3(1.0f, 0.5f, 0.31f));

		//渲染sun
		sunModel.render();
		
		//渲染moon和earth
		shader.enable();
		shader.setUniformMat4("projection_matrix", projection);
		shader.setUniformMat4("model_matrix", model);
		shader.setUniformMat4("view_matrix", view);
		//渲染moon
		model = mat4::scale(vec3(0.08, 0.08, 0.08)) * mat4::translation(vec3(0.3, -0.5, 0.3));
		shader.setUniformMat4("model_matrix", model);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		shader.setUniform1i("ourTexture", 1);
		moonModel.render();

		//渲染earth
		model = mat4::scale(vec3(0.08, 0.08, 0.08)) * mat4::translation(vec3(0.6, -0.5, 0.6));
		shader.setUniformMat4("model_matrix", model);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture3);
		shader.setUniform1i("ourTexture", 2);
		earthModel.render();

		window.update();
	}
	return 0;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
	Window *win = (Window *)glfwGetWindowUserPointer(window);
	camera.ProcessMouseScroll(yoffset);
}