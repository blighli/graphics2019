#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>

#define GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
	unsigned int ID;

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	const char* vertexSource;
	const char* fragmentSource;
	Shader(const char* vertexPath, const char* fragmentPath);

	void use();
	//~Shader();
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};


