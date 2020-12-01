#pragma once
class VertexArray {

private:

	unsigned int glID;

public:

	VertexArray();
	~VertexArray();

	void bind();
	void unbind();

};

