#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>
#include <model.h>
#include <sphere.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char *path);

// setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f)); // 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
//bool isPolyMode = false;

// timing
float lastFrame = 0.0f; // time of last frame
float deltaTime = 0.0f; // time between current frame and last frame

// positions of the point lights
glm::vec3 sunPosition = glm::vec3(0.0f, 0.0f, 0.0f);

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW_02", NULL, NULL);
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
	// -------------------------
	Shader sunShader("shaders\\sun.vs", "shaders\\sun.fs");
	Shader earthShader("shaders\\earth.vs", "shaders\\earth.fs");
	Shader moonShader("shaders\\moon.vs", "shaders\\moon.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes of lamps
	// ---------------------------------------------------------------------------
	Sphere sphere(32, 16);

	// configure the sphere's VAO, VBO, EBO
	unsigned int sphereVAO, sphereVBO, sphereEBO;
	glGenVertexArrays(1, &sphereVAO);
	glBindVertexArray(sphereVAO);

	glGenBuffers(1, &sphereVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.vertices.size() * sizeof(float), &sphere.vertices[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &sphereEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.indices.size() * sizeof(unsigned int), &sphere.indices[0], GL_STATIC_DRAW);
	
	float stride = (3 + 3 + 2) * sizeof(float);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	unsigned int sunDiffuseMap = loadTexture("resources\\objects\\sun\\8k_sun.jpg");
	unsigned int earthDiffuseMap = loadTexture("resources\\objects\\earth\\8k_earth_daymap.jpg");
	unsigned int earthSpecularMap = loadTexture("resources\\objects\\earth\\8k_earth_specular_map.jpg");
	unsigned int moonDiffuseMap = loadTexture("resources\\objects\\moon\\8k_moon.jpg");

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer

		// shader configuration (don't forget to enable shader before setting uniforms)
		// ----------------------------------------------------------------------------
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glBindVertexArray(sphereVAO); // fixed

		// sun
		// ---
		sunShader.use();
		glm::mat4 model = glm::mat4(1.0f);

		sunShader.setMat4("projection", projection);
		sunShader.setMat4("view", view);
		sunShader.setMat4("model", model);

		sunShader.setInt("diffuseTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sunDiffuseMap);
		
		glDrawElements(GL_TRIANGLES, sphere.indices.size(), GL_UNSIGNED_INT, (void*)0);

		// earth
		// -----
		earthShader.use();
		glm::vec3 earthPosition = glm::vec3(4.0f * glm::sin(currentFrame / 4.0f), 0.0f, 4.0f * glm::cos(currentFrame / 4.0f));
		model = glm::mat4(1.0f);
		model = glm::translate(model, earthPosition);
		model = glm::rotate(model, currentFrame / 2.0f, glm::vec3(0.0f, 0.5f, 0.1f));
		model = glm::scale(model, glm::vec3(0.5f));

		earthShader.setMat4("projection", projection);
		earthShader.setMat4("view", view);
		earthShader.setMat4("model", model);

		earthShader.setVec3("viewPos", camera.Position);

		earthShader.setVec3("pointLight.position", sunPosition);
		earthShader.setVec3("pointLight.ambient", 0.2f, 0.2f, 0.2f);
		earthShader.setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
		earthShader.setVec3("pointLight.specular", 0.1f, 0.1f, 0.1f);
		earthShader.setFloat("pointLight.constant", 1.0f);
		earthShader.setFloat("pointLight.linear", 0.09f);
		earthShader.setFloat("pointLight.quadratic", 0.032f);

		earthShader.setFloat("material.shininess", 4.0f);
		earthShader.setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, earthDiffuseMap);
		earthShader.setInt("material.specular", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, earthSpecularMap);

		glDrawElements(GL_TRIANGLES, sphere.indices.size(), GL_UNSIGNED_INT, (void*)0);

		// moon
		// ----
		moonShader.use();
		glm::vec3 moonPosition = earthPosition + glm::vec3(glm::sin(currentFrame), glm::sin(currentFrame) / 5.0f, glm::cos(currentFrame));
		model = glm::mat4(1.0f);
		model = glm::translate(model, moonPosition);
		model = glm::rotate(model, currentFrame, glm::vec3(0.5f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));

		moonShader.setMat4("projection", projection);
		moonShader.setMat4("view", view);
		moonShader.setMat4("model", model);

		moonShader.setVec3("viewPos", camera.Position);

		moonShader.setVec3("pointLight.position", sunPosition);
		moonShader.setVec3("pointLight.ambient", 0.2f, 0.2f, 0.2f);
		moonShader.setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
		moonShader.setVec3("pointLight.specular", 0.1f, 0.1f, 0.1f);
		moonShader.setFloat("pointLight.constant", 1.0f);
		moonShader.setFloat("pointLight.linear", 0.09f);
		moonShader.setFloat("pointLight.quadratic", 0.032f);

		moonShader.setFloat("material.shininess", 2.0f);
		moonShader.setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, moonDiffuseMap);

		glDrawElements(GL_TRIANGLES, sphere.indices.size(), GL_UNSIGNED_INT, (void*)0);

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

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(const char* path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		switch (nrComponents) {
		case 1:
			format = GL_RED; break;
		case 3:
			format = GL_RGB; break;
		case 4:
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}

	stbi_image_free(data);

	return textureID;
}