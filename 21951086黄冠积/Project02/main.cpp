#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "sphere.h"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <windows.h>
#include "LightPoint.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "camera.h"

float vertices[] = {

	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float lastX, lastY;
bool firstMouse = true;

Camera camera(glm::vec3(0, 0, 10.0f), glm::radians(0.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));

LightPoint lightP = LightPoint(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.7f, 0.7f, 0.7f));

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.UpdatePosition('W');
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.UpdatePosition('S');
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.UpdatePosition('A');
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.UpdatePosition('D');
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.UpdatePosition('Q');
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.UpdatePosition('E');
}

unsigned int LoadImageToGPU(const char* filename, GLint interalFormat, GLenum format, int textureSlot) {
	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, TexBuffer);

	int width, height, nrChannel;

	unsigned char* data = stbi_load(filename, &width, &height, &nrChannel, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, interalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "load image failed!" << std::endl;
	}
	stbi_image_free(data);
	return TexBuffer;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	float deltaX, deltaY;
	deltaX = xPos - lastX;
	deltaY = yPos - lastY;
	lastX = xPos;
	lastY = yPos;

	printf("%f %f\n", xPos, yPos);

	camera.ProcessMouseMovement(deltaX, deltaY);
}

int main() {


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Init GLFW failed.");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);

	//Shader testShader = Shader("vertexSource.txt", "fragmentSource.txt");
	Shader SunShader = Shader("sunshader.vert", "sunshader.frag");
	Shader EarthShader = Shader("earthshader.vert", "earthshader.frag");
	Shader MoonShader = Shader("moonshader.vert", "moonshader.frag");

	//stbi_set_flip_vertically_on_load(true);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	unsigned int TexBuffer_Sun;
	TexBuffer_Sun = LoadImageToGPU("sun.jpg", GL_RGB, GL_RGB, 0);
	unsigned int TexBuffer_Earth;
	TexBuffer_Earth = LoadImageToGPU("earth.jpg", GL_RGB, GL_RGB, 1);
	unsigned int TexBuffer_Moon;
	TexBuffer_Moon = LoadImageToGPU("moon.jpg", GL_RGB, GL_RGB, 2);

	Sphere mySphere(90, 45);
	// 创建太阳的顶点坐标以及法向量
	unsigned int sunVBO, sunVAO;
	glGenVertexArrays(1, &sunVAO);
	glGenBuffers(1, &sunVBO);
	glBindVertexArray(sunVAO);

	glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
	glBufferData(GL_ARRAY_BUFFER, mySphere.vertNum * 5 * sizeof(float), mySphere.verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 创建地球的顶点坐标以及法向量
	unsigned int earthVBO, earthVAO;
	glGenVertexArrays(1, &earthVAO);
	glGenBuffers(1, &earthVBO);
	glBindVertexArray(earthVAO);

	glBindBuffer(GL_ARRAY_BUFFER, earthVBO);
	glBufferData(GL_ARRAY_BUFFER, mySphere.vertNum * 5 * sizeof(float), mySphere.verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 创建月亮的顶点坐标以及法向量
	unsigned int moonVBO, moonVAO;
	glGenVertexArrays(1, &moonVAO);
	glGenBuffers(1, &moonVBO);
	glBindVertexArray(moonVAO);

	glBindBuffer(GL_ARRAY_BUFFER, moonVBO);
	glBufferData(GL_ARRAY_BUFFER, mySphere.vertNum * 5 * sizeof(float), mySphere.verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int time = 0;
	float theta;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 太阳
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexBuffer_Sun);

		SunShader.use();
		glBindVertexArray(sunVAO);
		glUniform1i(glGetUniformLocation(SunShader.ID, "texture1"), 0);

		glm::mat4 sunView;
		sunView = camera.GetViewMatrix();
		glm::mat4 sunProjection;
		sunProjection = glm::perspective(glm::radians(45.0f), 800 / 600.0f, 0.1f, 100.0f);
		glm::mat4 sunModel;
		glm::vec3 sunPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		sunModel = glm::translate(sunModel, sunPosition);

		glUniformMatrix4fv(glGetUniformLocation(SunShader.ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(sunModel));
		glUniformMatrix4fv(glGetUniformLocation(SunShader.ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(sunView));
		glUniformMatrix4fv(glGetUniformLocation(SunShader.ID, "projectionMat"), 1, GL_FALSE, glm::value_ptr(sunProjection));

		glBindVertexArray(sunVAO);
		glDrawArrays(GL_TRIANGLES, 0, mySphere.vertNum);


		// 地球
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TexBuffer_Earth);

		EarthShader.use();
		glBindVertexArray(earthVAO);
		glUniform1i(glGetUniformLocation(EarthShader.ID, "texture1"), 1);

		glm::mat4 earthView;
		earthView = camera.GetViewMatrix();
		glm::mat4 earthProjection;
		earthProjection = glm::perspective(glm::radians(45.0f), 800 / 600.0f, 0.1f, 100.0f);

		glm::mat4 earthModel;
		//glm::vec3 earthPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		//earthModel = glm::translate(earthModel, earthPosition);
		//earthModel = glm::translate(earthModel, glm::vec3(5.0f,0,5.0f));
		glm::vec3 earthPosition = glm::vec3(4.0*sin(int(time / 24) / (60 * 365.0f) * 2 * PI), 0.0f, 4.0*cos(int(time / 24) / (60 * 365.0f) * 2 * PI));
		earthModel = glm::translate(earthModel, earthPosition);
		earthModel = glm::rotate(earthModel, (float)(int(time / 60) % 24 / 24.0f * 2 * PI), glm::vec3(0.0f, 0.5f, 0.1f));
		earthModel = glm::scale(earthModel, glm::vec3(0.5, 0.5, 0.5));

		glUniformMatrix4fv(glGetUniformLocation(EarthShader.ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(earthModel));
		glUniformMatrix4fv(glGetUniformLocation(EarthShader.ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(earthView));
		glUniformMatrix4fv(glGetUniformLocation(EarthShader.ID, "projectionMat"), 1, GL_FALSE, glm::value_ptr(earthProjection));
		glUniform3f(glGetUniformLocation(EarthShader.ID, "LightP.LightPos"), lightP.position.x, lightP.position.y, lightP.position.z);
		glUniform3f(glGetUniformLocation(EarthShader.ID, "LightP.LightColor"), lightP.color.x, lightP.color.y, lightP.color.z);
		glUniform1f(glGetUniformLocation(EarthShader.ID, "LightP.constant"), lightP.constant);
		glUniform1f(glGetUniformLocation(EarthShader.ID, "LightP.linear"), lightP.linear);
		glUniform1f(glGetUniformLocation(EarthShader.ID, "LightP.quadratic"), lightP.quadratic);
		glUniform3f(glGetUniformLocation(EarthShader.ID, "earth_core_pos"), earthPosition.x, earthPosition.y, earthPosition.z);
		glUniform3f(glGetUniformLocation(EarthShader.ID, "ambident"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(EarthShader.ID, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		glBindVertexArray(earthVAO);
		glDrawArrays(GL_TRIANGLES, 0, mySphere.vertNum);

		// 月亮
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, TexBuffer_Moon);

		MoonShader.use();
		glBindVertexArray(moonVAO);
		glUniform1i(glGetUniformLocation(MoonShader.ID, "texture1"), 2);

		glm::mat4 moonView;
		moonView = camera.GetViewMatrix();
		glm::mat4 moonProjection;
		moonProjection = glm::perspective(glm::radians(45.0f), 800 / 600.0f, 0.1f, 100.0f);

		glm::mat4 moonModel;
		glm::vec3 moonPosition = glm::vec3(earthPosition.x + sin(int(time / 60) % (24 * 30) / (24 * 30.f) * 2 * PI), 0.0f, earthPosition.z + cos(int(time / 60) % (24 * 30) / (24 * 30.f) * 2 * PI));
		moonModel = glm::translate(moonModel, moonPosition);
		//moonModel = glm::rotate(moonModel, (float)(int(time / 100) % 24 / 24.0f * 2 * PI), glm::vec3(0.0f, 0.5f, 0.1f));
		moonModel = glm::scale(moonModel, glm::vec3(0.2, 0.2, 0.2));

		glUniformMatrix4fv(glGetUniformLocation(MoonShader.ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(moonModel));
		glUniformMatrix4fv(glGetUniformLocation(MoonShader.ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(moonView));
		glUniformMatrix4fv(glGetUniformLocation(MoonShader.ID, "projectionMat"), 1, GL_FALSE, glm::value_ptr(moonProjection));

		glUniform3f(glGetUniformLocation(EarthShader.ID, "LightP.LightPos"), lightP.position.x, lightP.position.y, lightP.position.z);
		glUniform3f(glGetUniformLocation(MoonShader.ID, "LightP.LightColor"), lightP.color.x, lightP.color.y, lightP.color.z);
		glUniform1f(glGetUniformLocation(MoonShader.ID, "LightP.constant"), lightP.constant);
		glUniform1f(glGetUniformLocation(MoonShader.ID, "LightP.linear"), lightP.linear);
		glUniform1f(glGetUniformLocation(MoonShader.ID, "LightP.quadratic"), lightP.quadratic);
		glUniform3f(glGetUniformLocation(MoonShader.ID, "moon_core_pos"), moonPosition.x, moonPosition.y, moonPosition.z);
		glUniform3f(glGetUniformLocation(MoonShader.ID, "earth_core_pos"), earthPosition.x, earthPosition.y, earthPosition.z);
		glUniform3f(glGetUniformLocation(MoonShader.ID, "sun_core_pos"), sunPosition.x, sunPosition.y, sunPosition.z);
		glUniform3f(glGetUniformLocation(MoonShader.ID, "ambident"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(MoonShader.ID, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		glBindVertexArray(moonVAO);
		glDrawArrays(GL_TRIANGLES, 0, mySphere.vertNum);



		time = time++ > 365 * 24 * 60 ? time - 365 * 24 * 60 : time;
		//printf("%d", time);
		//Sleep(30);
		//system("pause");
		glfwSwapBuffers(window);
		glfwPollEvents();

	}


	glfwTerminate();
	return 0;

}