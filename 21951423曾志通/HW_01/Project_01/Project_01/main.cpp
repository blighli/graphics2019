#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// Setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;
bool firstMouse = true;

// Timing
float lastFrame = 0.0f; // time of last frame
float deltaTime = 0.0f; // time between current frame and last frame

/**** ROTATE IT ****/
int rotateFlag[6]; // TO-DO: Later, i will hash the 27 small cubes to a 3x3x3 matrix, and apply rotation on the this matrix to simply the logic of position and cube entities.
// dim:		0 - Left;  1 - Right;  2 - Down;  3 - Up;  4 - Back;  5 - Front;
// value:	-1 - CW;  0 - Static;  1 - CCW;
bool isRotating = false; // This boolean variable plays a role of semaphore. When the magic cube is rotating, it must be locked.
int maxSteps = 50, stepsOfRotation = 0;
int directionOfRotation;
int phaseOfRotation = 0;

bool shiftLock = false;
// DAMN! This shiftLock method is ugly...
// The usual way to detect a MUTLIPLE-KEY-PRESS failed when the keys is 'R' and 'Shift'
// because the CW operator and CCW operator share the same key 'R' and the life of a frame is to short.
// And it isn't bug-free obviously, but just CURRENTLY.
/*******************/

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW_01", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//glfwSetKeyCallback(window, key_callback);

	// Tell GLFW to capture our mouse(the cursor will be hidden)
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	// ------------------------------------
	Shader ourShader("shader_vertex.vs", "shader_fragment.fs");

	// Set up vertex data and indices
	// ------------------------------
	float vertices[6][4 * 5] = { // 6x6=36 -> 6x4=24
		// Left
		{
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f
			//-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		},
		// Right
		{
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 //0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f
			 //0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		},
		// Down
		{
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 //0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f
			//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		},
		// Up
		{
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 //0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f
			//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		},
		// Back
		{
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // L D
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // R D
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // R U
			 //0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // R U
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f // L U
			//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // L D
		},
		// Front
		{
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 //0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f
			//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		}
	};
	unsigned int indices[6][6];
	for (unsigned int i = 0; i < 6; ++i) {
		indices[i][0] = 0;
		indices[i][1] = 1;
		indices[i][2] = 2;
		indices[i][3] = 2;
		indices[i][4] = 3;
		indices[i][5] = 0;
	}

	// World space positions of small cubes
	glm::vec3 cubePositions[27];
	for (unsigned int i = 0; i < 27; ++i) {
		int dx = i / 9 - 1;
		int dy = i % 9 / 3 - 1;
		int dz = i % 9 % 3 - 1;
		cubePositions[i] = glm::vec3(dx / 3.0f, dy / 3.0f, dz / 3.0f);
		// -- debug --
		//std::cout << "id " << i << ":\t" << cubePositions[i].x << " " << cubePositions[i].y << " " << cubePositions[i].z << std::endl;
	}

	// Bind the VAO first, then bind VBO and EBO, and configure vertex attributes.
	unsigned int VAOs[6], VBOs[6], EBOs[6];
	glGenVertexArrays(6, VAOs);
	glGenBuffers(6, VBOs);
	glGenBuffers(6, EBOs);
	for (unsigned int i = 0; i < 6; ++i) {
		glBindVertexArray(VAOs[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[i]), indices[i], GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texCoord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

	}
	// load and create a texture
	// -------------------------
	/*unsigned int texture = loadTexture("resources\\textures\\white.png");*/
	unsigned int colors[7];
	colors[0] = loadTexture("resources\\textures\\orange.png");	// 0 orange	Left	X-
	colors[1] = loadTexture("resources\\textures\\red.png");	// 1 red	Right	X+
	colors[2] = loadTexture("resources\\textures\\green.png");	// 2 green	Down	Y-
	colors[3] = loadTexture("resources\\textures\\blue.png");	// 3 blue	Up		Y+
	colors[4] = loadTexture("resources\\textures\\yellow.png");	// 4 yellow	Back	Z-
	colors[5] = loadTexture("resources\\textures\\white.png");	// 5 white	Front	Z+
	colors[6] = loadTexture("resources\\textures\\black.png");	// 6 black	core	O

	unsigned int cubeTextures[27][6];
	for (unsigned int i = 0; i < 27; ++i) {
		for (int dir = 2, id = i; dir >= 0; --dir, id /= 3) {
			int pos = id % 3 - 1;
			
			cubeTextures[i][dir * 2]	 = (pos == -1) ? colors[dir * 2] : colors[6];
			cubeTextures[i][dir * 2 + 1] = (pos == 1) ? colors[dir * 2 + 1] : colors[6];

			//switch (pos) {
			//case -1:
			//	cubeTextures[i][dir * 2] = colors[dir * 2];
			//	cubeTextures[i][dir * 2 + 1] = colors[6];
			//	break;
			//case 1:
			//	cubeTextures[i][dir * 2] = colors[6];
			//	cubeTextures[i][dir * 2 + 1] = colors[dir * 2 + 1];
			//	break;
			//default: // 0
			//	cubeTextures[i][dir * 2] = cubeTextures[i][dir * 2 + 1] = colors[6];
			//}
		}
	}

	// Tell OpenGL for each sampler to which texture unit it belongs (only has to be done once)
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
	ourShader.setInt("texture", 0);
	//glActiveTexture(GL_TEXTURE0);

	// Render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// Per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		// -----
		processInput(window);

		// Render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		// Activate shader
		ourShader.use();

		// Pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); // so the fov is fov of y-axis, and the width/height-ratio aka. aspect-ratio
		ourShader.setMat4("projection", projection);

		// Camera/View transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		// Render the Rubik's Cube
		
		/**** ROTATE IT ****/
		float rotAngle = 0.0f;
		if (isRotating) {
			++stepsOfRotation;
			rotAngle = glm::radians(90.0f * (phaseOfRotation + stepsOfRotation * 1.0 / maxSteps * directionOfRotation));
			if (stepsOfRotation == maxSteps) {
				stepsOfRotation = 0;
				isRotating = false;
				phaseOfRotation = (phaseOfRotation + directionOfRotation) % 4; // + 1 * direction
				shiftLock = false; // hey, i am here ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			}
		} else {
			rotAngle = glm::radians(90.0f * phaseOfRotation);
		}
		/*******************/

		for (unsigned int i = 0; i < 27; ++i) {
			for (unsigned int j = 0; j < 6; ++j) {

				glBindTexture(GL_TEXTURE_2D, cubeTextures[i][j]);

				glBindVertexArray(VAOs[j]);
				glm::mat4 model = glm::mat4(1.0f);

				/**** ROTATE IT ****/
				if (i / 9 == 2) {
					model = glm::rotate(model, rotAngle, glm::vec3(1.0f, 0.0f, 0.0f));
				}
				/*******************/

				model = glm::translate(model, cubePositions[i]);
				model = glm::scale(model, glm::vec3(1 / 3.0f));
				ourShader.setMat4("model", model);

				//glDrawArrays(GL_TRIANGLES, 0, 36);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Optional: de-allocate all resources once they've outlived their purpose
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(6, VAOs);
	glDeleteBuffers(6, VBOs);
	glDeleteBuffers(6, EBOs);
	glDeleteTextures(7, colors);

	// glfw: terminate, clearing all previously allocated GLFW resources
	// -----------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
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


	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !shiftLock) {
		shiftLock = true;
		//std::cout << "LEFT_SHIFT" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !isRotating) {
		isRotating = true;
		//directionOfRotation = 1;
		directionOfRotation = shiftLock ? -1 : 1;
		//shiftLock = false;
	}

	//if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !isRotating) {
	//	isRotating = true;
	//	directionOfRotation = -1;
	//}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// Make sure the viewport matches the new window dimensions.
	// Note that width and height will be significantly larger than specified on retina displayers.
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
	float yoffset = lastY - ypos; // reserved since y-coordinates go from bottom to top, while the pixers go oppositely

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

// Utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(const char* path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
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

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//
//}