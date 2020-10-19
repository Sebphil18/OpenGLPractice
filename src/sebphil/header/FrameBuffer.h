#pragma once

class FrameBuffer {

private:

	unsigned int glID;

	bool framebufferIsValid();
	bool framebufferIsComplete();
	void printErrMsg();

public:

	FrameBuffer();
	~FrameBuffer();

	void bind();
	void unbind();

	void attachColorTexture2D(unsigned int textureID, unsigned int colorAttachment);
	void attachStencilDepthBuffer(unsigned int renderBufferID);

};