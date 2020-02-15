#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>
#include <model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
//bool isPolyMode = false;

// timing
float lastFrame = 0.0f; // time of last frame
float deltaTime = 0.0f; // time between current frame and last frame

int main() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // fix compilation on OS X
#endif // __APPLE__

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW_03", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse(the cursor will be hidden)
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global OpenGL state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// ------------------------------------
	Shader ourShader("model.vs", "model.fs");
	Shader lampShader("lamp.vs", "lamp.fs");
	Shader squareShader("square.vs", "square.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes of lamps
// -------------------------------------------------------------------------------
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	// positions of the point lights
	int nrPointLights = 4;
	glm::vec3 pointLightPositions[] = {
		//glm::vec3(1.0f, 2.0f, 1.0f),
		//glm::vec3(1.0f, 2.0f, -1.0f),
		//glm::vec3(-1.0f, 2.0f, 1.0f),
		//glm::vec3(-1.0f, 2.0f, -1.0f),

		glm::vec3(1.0f, -2.0f, 1.0f),
		glm::vec3(1.0f, -2.0f, -1.0f),
		glm::vec3(-1.0f, -2.0f, 1.0f),
		glm::vec3(-1.0f, -2.0f, -1.0f)
	};
	glm::vec3 pointLightColors[] = {
		glm::vec3(0.8f, 0.8f, 0.2f),
		glm::vec3(0.2f, 0.8f, 0.2f),
		glm::vec3(0.2f, 0.2f, 0.8f),
		glm::vec3(0.8f, 0.2f, 0.2f)
	};

	// first, configure the cube's VAO (and VBO)
	unsigned int lightVAO, lightVBO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Square
	float square[] = {
		// positions		// colors
		 0.5f,  0.0f,  0.5f,  0.8f, 0.8f, 0.2f,
		 0.5f,  0.0f, -0.5f,  0.2f, 0.8f, 0.2f,
		-0.5f,  0.0f,  0.5f,  0.2f, 0.2f, 0.8f,
		-0.5f,  0.0f, -0.5f,  0.8f, 0.2f, 0.2f,
		 0.0f,  0.0f,  0.0f,  0.5f, 0.5f, 0.5f
	/*	 1.0f, -2.0f,  1.0f,  0.8f, 0.8f, 0.2f, 
		 1.0f, -2.0f, -1.0f,  0.2f, 0.8f, 0.2f,
		-1.0f, -2.0f,  1.0f,  0.2f, 0.2f, 0.8f,
		-1.0f, -2.0f, -1.0f,  0.8f, 0.2f, 0.2f*/
	};
	unsigned int indices[] = {
		0, 1, 4,
		1, 3, 4,
		3, 2, 4,
		2, 0, 4
	};
	unsigned int squareVAO, squareVBO, squareEBO;
	glGenVertexArrays(1, &squareVAO);
	glBindVertexArray(squareVAO);
	glGenBuffers(1, &squareVBO);
	glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
	glGenBuffers(1, &squareEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// load models
	// -----------
	Model ourModel("resources/objects/nanosuit/nanosuit.obj");

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// shader configuration
	// --------------------
	ourShader.use();
	// material properties
	ourShader.setFloat("material.shininess", 32.0f);
	// light properties
	// directional light
	ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	ourShader.setVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
	ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	// point light
	for (unsigned int i = 0; i < nrPointLights; ++i) {
		ourShader.setVec3(("pointLights[" + to_string(i) + "].position").c_str(), pointLightPositions[i]);
		ourShader.setVec3("pointLights[" + to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLights[" + to_string(i) + "].diffuse", pointLightColors[i]); // 0.8
		ourShader.setVec3("pointLights[" + to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pointLights[" + to_string(i) + "].constant", 1.0f);
		ourShader.setFloat("pointLights[" + to_string(i) + "].linear", 0.09);
		ourShader.setFloat("pointLights[" + to_string(i) + "].quadratic", 0.032);
	}

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		// spotLight
		ourShader.setVec3("spotLight.position", camera.Position);
		ourShader.setVec3("spotLight.direction", camera.Front);
		ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		ourShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("spotLight.constant", 1.0f);
		ourShader.setFloat("spotLight.linear", 0.09f);
		ourShader.setFloat("spotLight.quadratic", 0.032f);
		ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(2.0f))); // 12.5
		ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(4.0f))); // 15.0


		ourShader.setVec3("viewPos", camera.Position);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::translate(model, glm::vec3(0.0f, -1.75f - 0.25f * abs(sin(glfwGetTime() / 2)), 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f)); // it's a bit too big for our scene, so scale it down
		model = glm::scale(model, glm::vec3(0.2f - 0.2f * abs(sin(glfwGetTime() / 2)))); // it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);

		// render the loaded model
		ourModel.Draw(ourShader);

		// also draw the lamp object(s)
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < nrPointLights; ++i) {
			lampShader.setVec3("LampColor", pointLightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
			lampShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		squareShader.use();
		squareShader.setMat4("projection", projection);
		squareShader.setMat4("view", view); // -1.0f, -2.0f, -1.0f
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.8f));
		squareShader.setMat4("model", model);
		glBindVertexArray(squareVAO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources
	// -----------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	//if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
	//	if (isPolyMode) {
	//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//		isPolyMode = false;
	//	} else {
	//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//		isPolyMode = true;
	//	}
	//}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions
	// note that width and height will be significantly larger than specified on retina displayers
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reserved since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}