#include "header/VertexArray.h"
#include "glad/glad.h"

VertexArray::VertexArray() {

	glGenVertexArrays(1, &glID);

}

VertexArray::~VertexArray() {

	glDeleteVertexArrays(1, &glID);
	glID = 0;

}

void VertexArray::bind() {

	glBindVertexArray(glID);

}

void VertexArray::unbind() {

	glBindVertexArray(0);

}