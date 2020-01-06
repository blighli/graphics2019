#pragma once
#include <GL/glew.h>

class IndexBuffer {
private:
	GLuint m_BufferID;
	GLuint m_Count;

public:
	IndexBuffer();
	IndexBuffer(GLushort *data, GLsizei count);
	~IndexBuffer();
	void bind() const;
	void unbind() const;

	GLuint getCount() const { return m_Count; }
};