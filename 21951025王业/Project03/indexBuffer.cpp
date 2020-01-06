#include "indexBuffer.h"

IndexBuffer::IndexBuffer(GLushort *data, GLsizei count):m_Count(count){
	glGenBuffers(1, &m_BufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLushort), data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
IndexBuffer::IndexBuffer() {
	m_BufferID = 0;
	m_Count = 0;
}
IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &m_BufferID);
}
void IndexBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
}
void IndexBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}