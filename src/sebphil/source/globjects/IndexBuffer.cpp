#include "globjects/IndexBuffer.h"
#include "glad/glad.h"

IndexBuffer::IndexBuffer(): count(0) {
	generateBuffer();
}

IndexBuffer::IndexBuffer(unsigned int count, const void* data, unsigned int usage) {
	generateBuffer();
	bind();
	setData(count, data, usage);
	unbind();
}

void IndexBuffer::generateBuffer() {
	glGenBuffers(1, &glID);
}

IndexBuffer::~IndexBuffer() {
	deleteBuffer();
	glID = 0;
}

void IndexBuffer::deleteBuffer() {
	glDeleteBuffers(1, &glID);
}

void IndexBuffer::setData(unsigned int size, const void* data, unsigned int usage) {

	this->count = size / sizeof(unsigned int);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);

}

void IndexBuffer::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glID);
}

void IndexBuffer::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::getCount() const {
	return count;
}