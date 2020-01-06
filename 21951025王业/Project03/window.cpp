#include "window.h"
#include <iostream>
using namespace std;

void window_resize_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

bool Window::init() {
	glfwInit();
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
	if (m_Window == NULL) {
		std::cout << "Window created Failed!" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetWindowSizeCallback(m_Window, window_resize_callback);
	glfwSetKeyCallback(m_Window, key_callback);
	glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
	glfwSetCursorPosCallback(m_Window, cursor_position_callback);

	glewInit();

	//std::cout << "OPENGL" << glGetString(GL_VERSION) << std::endl;
	return true;
}

Window::Window(const char *title, int width, int height) {
	m_Title = title;
	m_Width = width;
	m_Height = height;

	if (!init()) {
		glfwTerminate();
	}
	for (int i = 0; i < MAX_KEYS; i++) {
		m_Keys[i] = false;
	}
	for (int i = 0; i < MAX_BUTTONS; i++) {
		m_MouseButtons[i] = false;
	}
}

Window::~Window() {
	glfwTerminate();
}

bool Window::closed() const {
	return glfwWindowShouldClose(m_Window);
}
void Window::update() {
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}
void Window::clear() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::isKeyPressed(unsigned int keycode) const {
	if (keycode >= MAX_KEYS) return false;
	return m_Keys[keycode];
}
bool Window::isMouseButtonPressed(unsigned int button) const {
	if (button >= MAX_BUTTONS) return false;
	return m_MouseButtons[button];
}
void Window::getCursorPosition(double &x, double &y) const {
	x = m_X;
	y = m_Y;
	//cout << "x: " << x << " y: " << y << endl;
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	Window *win = (Window *)glfwGetWindowUserPointer(window);
	win->m_Keys[key] = action != GLFW_RELEASE;
}
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	Window *win = (Window *)glfwGetWindowUserPointer(window);
	win->m_MouseButtons[button] = action != GLFW_RELEASE;
}
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
	Window *win = (Window *)glfwGetWindowUserPointer(window);
	win->m_X = xpos;
	win->m_Y = ypos;
}
