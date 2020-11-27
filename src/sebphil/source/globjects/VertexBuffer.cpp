#include "globjects/VertexBuffer.h"

VertexBuffer::VertexBuffer() {
	generateBuffer();
}

VertexBuffer::VertexBuffer(const unsigned int sizeBytes, const void* data, const unsigned int usage) {
	generateBuffer();
	bind();
	setData(sizeBytes, data, usage);
	unbind();
}

void VertexBuffer::generateBuffer() {
	glGenBuffers(1, &glID);
}

VertexBuffer::~VertexBuffer() {
	deleteBuffer();
	glID = 0;
}

void VertexBuffer::deleteBuffer() {
	glDeleteBuffers(1, &glID);
}

void VertexBuffer::bind() {

	glBindBuffer(GL_ARRAY_BUFFER, glID);

}

void VertexBuffer::unbind() {

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void VertexBuffer::setData(const unsigned int sizeBytes, const void* data, const unsigned int usage) {

	glBufferData(GL_ARRAY_BUFFER, sizeBytes, data, usage);

}