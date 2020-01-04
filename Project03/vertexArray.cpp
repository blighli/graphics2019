#include "vertexArray.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_ArrayID);
}

VertexArray::~VertexArray() {
	for (int i = 0; i < m_Buffers.size(); i++)
		delete m_Buffers[i];
	glDeleteVertexArrays(1, &m_ArrayID);
}

void VertexArray::bind() const {
	glBindVertexArray(m_ArrayID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

void VertexArray::addBuffer(Buffer *buffer, GLuint index) {
	bind();
	buffer->bind();

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, 0, 0);
	m_Buffers.push_back(buffer);

	buffer->unbind();
	unbind();
}