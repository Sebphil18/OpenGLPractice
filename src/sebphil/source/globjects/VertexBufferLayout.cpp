#include "globjects/VertexBufferLayout.h"
#include "glad/glad.h"

VertexBufferLayout::VertexBufferLayout() {

	stride = 0;
	elements.reserve(3);

}

std::size_t VertexBufferLayout::addElement(const LayoutElement& element) {

	elements.push_back(element);
	
	addElementToStride(element);

	return elements.size();
}

void VertexBufferLayout::addElementToStride(const LayoutElement& element) {
	stride += element.count * getSizeOfType(element.type);
}

void VertexBufferLayout::removeElement(std::size_t index) {

	const LayoutElement& element = elements[index];
	
	removeElementFromStride(element);
	
	elements.erase(elements.begin() + index);

}

void VertexBufferLayout::removeElementFromStride(const LayoutElement& element) {
	stride -= element.count * getSizeOfType(element.type);
}

void VertexBufferLayout::bindLayout() {

	unsigned int offset = 0;
	
	for (size_t i = 0; i < elements.size(); i++) {

		const LayoutElement& element = elements[i];
		
		addVertexAttribute(element, offset, i);

		offset += getAttributeOffset(element);

	}

}

void VertexBufferLayout::addVertexAttribute(const LayoutElement& element, unsigned int offset, std::size_t index) {
	defineVertexAttribute(element, offset, index);
	enableVertexAttribute(index);
}

void VertexBufferLayout::defineVertexAttribute(const LayoutElement& element, unsigned int offset, std::size_t index) {
	glVertexAttribPointer(index, element.count, element.type, element.normalized, stride, (void*) offset);
}

void VertexBufferLayout::enableVertexAttribute(std::size_t index) {
	glEnableVertexAttribArray(index);
}

unsigned int VertexBufferLayout::getAttributeOffset(const LayoutElement& element) {
	return element.count * getSizeOfType(element.type);
}

unsigned int VertexBufferLayout::getSizeOfType(unsigned int type) {
	
	unsigned int size;
	
	switch (type) {

	case GL_FLOAT:

		size = sizeof(float);
		break;

	case GL_DOUBLE:

		size = sizeof(double);
		break;

	case GL_INT:

		size = sizeof(int);
		break;

	case GL_UNSIGNED_INT:

		size = sizeof(unsigned int);
		break;

	default:
		size = sizeof(type);
	}

	return size;
}
