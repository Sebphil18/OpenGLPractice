#pragma once
#include "glad/glad.h"

class VertexBuffer {

private:

	unsigned int glID;

	void generateBuffer();
	void deleteBuffer();

public:

	VertexBuffer();
	VertexBuffer(const unsigned int sizeBytes, const void* data, const unsigned int usage);
	~VertexBuffer();

	void bind();
	void unbind();

	void setData(const unsigned int sizeBytes, const void* data, const unsigned int usage);

};

