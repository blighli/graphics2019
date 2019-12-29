#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/shader.h"
#include "headers/stb_image.h"
#include "headers/camera.h"
#include "headers/sphere.h"


//声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

Camera mycamera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solaries-21951429", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader zprogram
	// ------------------------------------
	Shader sunShader("shaders/sunshader.vs", "shaders/sunshader.fs");
	Shader earthShader("shaders/earthshader.vs", "shaders/earthshader.fs");
	Shader moonShader("shaders/moonshader.vs", "shaders/moonshader.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	Sphere mySphere(90, 45);


	//sun
	//--------------------------------
	unsigned int sunVBO,sunVAO;
	glGenVertexArrays(1, &sunVAO);
	glGenBuffers(1, &sunVBO);

	glBindVertexArray(sunVAO);

	glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
	glBufferData(GL_ARRAY_BUFFER, mySphere.vertNum*5*sizeof(float), mySphere.verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//earth & moon
	unsigned int earthVBO, earthVAO;
	glGenVertexArrays(1, &earthVAO);
	glGenBuffers(1, &earthVBO);

	glBindBuffer(GL_ARRAY_BUFFER, earthVBO);
	glBufferData(GL_ARRAY_BUFFER, mySphere.vertNum * 5 * sizeof(float), mySphere.verts, GL_STATIC_DRAW);

	glBindVertexArray(earthVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	
	//Generate Texture
	//---------------------------------------------------
	unsigned int sunTexture = loadTexture("textures/sun.jpg");
	unsigned int earthDiffuse = loadTexture("textures/earth.jpg");
	unsigned int moonDiffuse = loadTexture("textures/moon.jpg");

	
	glEnable(GL_DEPTH_TEST);
	
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// init
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float programTime = (float)glfwGetTime();

		//earth
		//-----------------------------
		earthShader.use();
		earthShader.setVec3("light.position", lightPos);
		earthShader.setVec3("viewPos", mycamera.Position);

		earthShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		earthShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
		//earthShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		//earthShader.setFloat("material.shininess", 64.0f);

		glm::mat4 earthProjection = glm::perspective(glm::radians(mycamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 earthView = mycamera.GetViewMatrix();
		earthShader.setMat4("projection", earthProjection);
		earthShader.setMat4("view", earthView);

		// world transformation   glm变换矩阵：代表把当前矩阵与这个变换矩阵相乘
		glm::mat4 earthModel;
		glm::vec3 earthPosition = glm::vec3(4.0*sin(0.2*programTime+1), 0.0f, 4.0*cos(0.2*programTime+1) );		
		earthModel= glm::translate(earthModel, earthPosition);
		earthModel = glm::rotate(earthModel, (float)(programTime*0.5), glm::vec3(0.0f, 0.5f, 0.1f));
		earthModel = glm::scale(earthModel, glm::vec3(0.5, 0.5, 0.5));
		earthShader.setMat4("model", earthModel);

		// bind diffuse map
		earthShader.setInt("diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, earthDiffuse);
		// bind specular map
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, specularMap);

		// render the cube
		glBindVertexArray(earthVAO);
		glDrawArrays(GL_TRIANGLES, 0, mySphere.vertNum);

		//moon
		//-----------------------------
		moonShader.use();
		moonShader.setVec3("light.position", lightPos);
		moonShader.setVec3("viewPos", mycamera.Position);

		moonShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		moonShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		//moonShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		//moonShader.setFloat("material.shininess", 64.0f);

		glm::mat4 moonProjection = glm::perspective(glm::radians(mycamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 moonView = mycamera.GetViewMatrix();
		moonShader.setMat4("projection", moonProjection);
		moonShader.setMat4("view", moonView);

		// world transformation   glm变换矩阵：代表把当前矩阵与这个变换矩阵相乘
		glm::mat4 moonModel;
		glm::vec3 moonPosition = glm::vec3(1.0*sin(0.1*programTime)+earthPosition.x, 0.2*sin(0.1*programTime)+earthPosition.y, 1.0*cos(0.1*programTime)+earthPosition.z);
		moonModel = glm::translate(moonModel, moonPosition);
		moonModel = glm::rotate(moonModel, (float)(programTime*0.3), glm::vec3(0.5f, 0.5f, 0.0f));
		moonModel = glm::scale(moonModel, glm::vec3(0.2, 0.2, 0.2));
		moonShader.setMat4("model", moonModel);

		// bind diffuse map
		moonShader.setInt("diffuse", 0);
		glActiveTexture(GL_TEXTURE0);	
		glBindTexture(GL_TEXTURE_2D, moonDiffuse);
		// bind specular map
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, specularMap);

		// render the cube
		glBindVertexArray(earthVAO);
		glDrawArrays(GL_TRIANGLES, 0, mySphere.vertNum);

		//sun
		//-----------------------------
		sunShader.use();
		sunShader.setInt("texture1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sunTexture);
		
		glm::mat4 sunView;
		sunView = mycamera.GetViewMatrix();
		glm::mat4 sunProjection;
		sunProjection = glm::perspective(glm::radians(mycamera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		sunShader.setMat4("view", sunView);
		sunShader.setMat4("projection", sunProjection);

		glm::mat4 sunModel;
		glm::vec3 sunPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		sunModel = glm::translate(sunModel, sunPosition);		
		sunShader.setMat4("model", sunModel);

		glBindVertexArray(sunVAO);
		glDrawArrays(GL_TRIANGLES, 0, mySphere.vertNum);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	
	glDeleteVertexArrays(1, &sunVAO);
	glDeleteBuffers(1, &sunVBO);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mycamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mycamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mycamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mycamera.ProcessKeyboard(RIGHT, deltaTime);
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	mycamera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mycamera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}