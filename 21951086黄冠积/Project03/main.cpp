#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glut/glut.h>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// light
LightDirectional lightD = LightDirectional(glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(0, glm::radians(90.0f), 0), glm::vec3(10.0f, 0.0f, 0.0f));
LightPoint lightP = LightPoint(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0, 0, 0), glm::vec3(0.0f, 10.0f, 0.0f));
LightSpot lightS = LightSpot(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0, glm::radians(180.0f), 0), glm::vec3(0.0f, 0.0f, 10.0f));

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 滚轮是否代表放大缩小:1，或者代表镜头的拉近拉远
bool roll_enlarge = true;
// 放大的倍数
float size_x = 0.5, size_y = 0.5, size_z = 0.5;
// 旋转
float rotate_angle = 0;
float rotate_sensity;
// 平移
float pos_x = 0;
float pos_y = 0;
float pos_z = 0;
float pos_sensity = 0.003;

// 鼠标
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraDistanceX;
float cameraDistanceY;
float cameraAngleX;
float cameraAngleY;
float times = 1;


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glCullFace(GL_FRONT);

	// build and compile shaders
	// -------------------------
	Shader ourShader("1.model_loading.vert", "1.model_loading.frag");

	// load models
	// -----------
	//Model ourModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
	//Model ourModel("./resources/objects/nanosuit/nanosuit.obj");
	//Model ourModel("C:\\Users\\Ji\\source\\repos\\HomeWork_01\\HomeWork_01\\Debug\\resources\\objects\\talbot\\source\\TLHR\\TLHR.obj");


	// 有效的
	Model ourModel("C:\\Users\\Ji\\source\\repos\\HomeWork_01\\HomeWork_01\\Debug\\resources\\objects\\alien\\source\\PR_OBJ.obj");
	//Model ourModel("C:\\Users\\Ji\\source\\repos\\HomeWork_01\\HomeWork_01\\Debug\\resources\\objects\\nanosuit\\nanosuit.obj");



	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
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
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(pos_x, pos_y, pos_z));
		//model = glm::translate(model, glm::vec3(0.0f, -0.0f, 0.0f));
		rotate_angle += rotate_sensity;
		model = glm::rotate(model, glm::radians(rotate_angle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(size_x, size_y, size_z));
		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);

		glUniform3f(glGetUniformLocation(ourShader.ID, "lightP.Pos"), lightP.position.x, lightP.position.y, lightP.position.z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "lightP.color"), lightP.color.x, lightP.color.y, lightP.color.z);
		glUniform1f(glGetUniformLocation(ourShader.ID, "lightP.constant"), lightP.constant);
		glUniform1f(glGetUniformLocation(ourShader.ID, "lightP.linear"), lightP.linear);
		glUniform1f(glGetUniformLocation(ourShader.ID, "lightP.quadratic"), lightP.quadratic);

		glUniform3f(glGetUniformLocation(ourShader.ID, "lightD.Pos"), lightD.position.x, lightD.position.y, lightD.position.z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "lightD.dirToLight"), lightD.direction.x, lightD.direction.y, lightD.direction.z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "lightD.color"), lightD.color.x, lightD.color.y, lightD.color.z);

		glUniform3f(glGetUniformLocation(ourShader.ID, "lightS.Pos"), lightS.position.x, lightS.position.y, lightS.position.z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "lightS.dirToLight"), lightS.direction.x, lightS.direction.y, lightS.direction.z);
		glUniform3f(glGetUniformLocation(ourShader.ID, "lightS.color"), lightS.color.x, lightS.color.y, lightS.color.z);
		glUniform1f(glGetUniformLocation(ourShader.ID, "lightS.cosInnerPhy"), lightS.cosInnerPhy);
		glUniform1f(glGetUniformLocation(ourShader.ID, "lightS.cosOutterPhy"), lightS.cosOutterPhy);
		glUniform1f(glGetUniformLocation(ourShader.ID, "lightS.constant"), lightS.constant);
		glUniform1f(glGetUniformLocation(ourShader.ID, "lightS.linear"), lightS.linear);
		glUniform1f(glGetUniformLocation(ourShader.ID, "lightS.quadratic"), lightS.quadratic);
		glUniform3f(glGetUniformLocation(ourShader.ID, "ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(ourShader.ID, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);



		ourModel.Draw(ourShader);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
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
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(Q, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(L, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pos_x -= pos_sensity;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pos_x += pos_sensity;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pos_z -= pos_sensity;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pos_z += pos_sensity;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		pos_y -= pos_sensity;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		pos_y += pos_sensity;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (!roll_enlarge)
	{
		camera.ProcessMouseScroll(yoffset);
	}
	else {
		float sensity = 0.01;
		size_x += yoffset * sensity;
		size_y += yoffset * sensity;
		size_z += yoffset * sensity;
		size_x = size_x <= 0.01 ? 0.01 : size_x;
		size_y = size_y <= 0.01 ? 0.01 : size_y;
		size_z = size_z <= 0.01 ? 0.01 : size_z;
	}
}


//  鼠标左右键控制旋转

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	std::cout << "hello" << std::endl;
	if (action == GLFW_PRESS)
		rotate_sensity = 0.05;
	else if (action == GLFW_RELEASE)
		rotate_sensity = 0;
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		//rotate_angle += sensity;
		rotate_sensity = rotate_sensity > 0 ? rotate_sensity : -rotate_sensity;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		rotate_sensity = rotate_sensity < 0 ? rotate_sensity : -rotate_sensity;
		break;
	default:
		return;
		}
	std::cout << "hello2" << std::endl;
}


