#include "globjects/FrameBuffer.h"
#include "glad/glad.h"

#include <iostream>

FrameBuffer::FrameBuffer() {
	glGenFramebuffers(1, &glID);
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &glID);
}

void FrameBuffer::bind() {

	if (framebufferIsValid())
		glBindFramebuffer(GL_FRAMEBUFFER, glID);
	else
		std::cout << "ERROR::FRAMEBUFFER::Framebuffer is incomplete! " << std::endl;
}

bool FrameBuffer::framebufferIsValid() {
	if (framebufferIsComplete())
		return true;
	else {
		printErrMsg();
		return false;
	}
}

bool FrameBuffer::framebufferIsComplete() {
	return glCheckFramebufferStatus(glID) != GL_FRAMEBUFFER_COMPLETE;
}

void FrameBuffer::printErrMsg() {

	unsigned int status = glCheckFramebufferStatus(glID);

	switch (status) {

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		std::cout << "ERROR::FRAMEBUFFER::Framebuffer has an incomplete attachment!" << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		std::cout << "ERROR::FRAMEBUFFER::Framebuffer has an incomplete draw-buffer!" << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		std::cout << "ERROR::FRAMEBUFFER::Framebuffer has no image attachment!" << std::endl;
		break;
	default:
		std::cout << "ERROR::FRAMEBUFFER::Framebuffer is incomplete!" << std::endl;

	}

}

void FrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::attachColorTexture2D(unsigned int textureID, unsigned int colorAttachment) {
	glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachment, GL_TEXTURE_2D, textureID, 0);
}

void FrameBuffer::attachStencilDepthBuffer(unsigned int renderBufferID) {
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);
}