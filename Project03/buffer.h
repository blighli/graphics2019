#pragma once

#include <GL/glew.h>

class Buffer {
private:
	GLuint m_BufferID;
	GLuint m_ComponentCount;
public:
	Buffer(GLfloat *data, GLsizei count, GLuint componentCount);
	~Buffer();
	void bind() const;
	void unbind() const;

	GLuint getComponentCount() const { return m_ComponentCount; }
};