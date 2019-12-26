#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include "Vector.h"

class Shader {
private:
	GLuint m_ShaderID;
	const char *m_VertPath, *m_FragPath;

public:
	Shader(const char *vertexPath, const char *fragmengPath);
	~Shader();

	void setUniform1f(const GLchar *name, float value);
	void setUniform1i(const GLchar *name, int value);
	void setUniform2f(const GLchar *name, const vec2 &v);
	void setUniform3f(const GLchar *name, const vec3 &v);
	void setUniform4f(const GLchar *name, const vec4 &v);

	void setUniformMat4(const GLchar *name, const mat4 &matrix);

	GLuint getID();

	void enable() const;
	void disable() const;

private:
	GLuint load();
	GLint getUniformLocation(const GLchar *name);
};