#include "globjects/RenderBuffer.h"
#include "glad/glad.h"

RenderBuffer::RenderBuffer() {
	generateRenderbuffer();
}

RenderBuffer::RenderBuffer(unsigned int format, unsigned int width, unsigned int height) {

	generateRenderbuffer();
	bind();
	setData(format, width, height);
	unbind();

}

void RenderBuffer::generateRenderbuffer() {
	glCreateBuffers(1, &glID);
}

void RenderBuffer::bind() {
	glBindBuffer(GL_RENDERBUFFER, glID);
}

void RenderBuffer::setData(unsigned int format, unsigned int width, unsigned int height) {
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
}

void RenderBuffer::unbind() {
	glBindBuffer(GL_RENDERBUFFER, 0);
}

RenderBuffer::~RenderBuffer() {
	glDeleteBuffers(1, &glID);
}

unsigned int RenderBuffer::getID() const {
	return glID;
}