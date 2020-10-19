#include "header/UniformBuffer.h"
#include <iostream>
#include "glad/glad.h"

std::size_t UniformBuffer::count = 0;

UniformBuffer::UniformBuffer(unsigned int length, std::size_t size): length(length)
{
	generateBuffer();
	fillSizes(length);
	setSlot();

	bind();
	setData(size);
	bindBufferToSlot();
	unbind();

}

void UniformBuffer::generateBuffer() {
	glGenBuffers(1, &glID);
}

void UniformBuffer::fillSizes(unsigned int length) {

	elements.reserve(length);

	for (size_t i = 0; i < length; i++)
		elements.emplace_back(UniformBufferElement{0, 0, 0});

}

void UniformBuffer::setSlot() {

	slot = count;
	count++;

}

void UniformBuffer::bind() {
	glBindBuffer(GL_UNIFORM_BUFFER, glID);
}

void UniformBuffer::setData(std::size_t size) {
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
}

void UniformBuffer::bindBufferToSlot() {
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, glID);
}

void UniformBuffer::unbind() {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::~UniformBuffer() {
	deleteBuffer();
}

void UniformBuffer::deleteBuffer() {

	glDeleteBuffers(1, &glID);
	glID = 0;

}

void UniformBuffer::setElementType(std::size_t index, UniformType type) {

	if (index < length)
		setElementSize(elements[index], type);
	else {
		std::cout << "ERROR::UNIFORMBUFFER::Index " << index << " is out of bounds! ";
		std::cout << "The highest, valid index is " << length - 1 << "." << std::endl;
	}

}

void UniformBuffer::setElementSize(UniformBufferElement& element, UniformType typeOfElement) {
	element.size = getSizeOfType(typeOfElement);
}

unsigned int UniformBuffer::getSizeOfType(UniformType type) {

	unsigned int size = 0;

	switch (type) {

	case UniformType::integer:
		size = 4;
		break;

	case UniformType::decimal:
		size = 4;
		break;

	case UniformType::vec2D:
		size = 8;
		break;

	case UniformType::vec3D:
		size = 16;
		break;

	case UniformType::vec4D:
		size = 16;
		break;

	case UniformType::matrix4:
		size = 64;
		break;

	default:
		size = 0;
		break;

	}

	return size;
}

// TODO: unclear & to long :(
void UniformBuffer::setElementData(std::size_t index, const void* data) {

	if (index < length) {
		UniformBufferElement& element = elements[index];
		element.data = data;
		element.offset = getOffsetToElement(index);

		setSubData(element);
	} else {

		std::cout << "ERROR::UNIFORMBUFFER::Index " << index << " is out of bounds! ";
		std::cout << "The highest, valid index is " << length - 1 << "." << std::endl;

	}

}

unsigned int UniformBuffer::getOffsetToElement(std::size_t indexOfElement) {

	unsigned int offset = 0;

	for (size_t i = 0; i < indexOfElement; i++)
		offset += elements[i].size;

	return offset;

}

void UniformBuffer::setSubData(const UniformBufferElement& element) {
	glBufferSubData(GL_UNIFORM_BUFFER, element.offset, element.size, element.data);
}

unsigned int UniformBuffer::getID() const {
	return glID;
}

std::size_t UniformBuffer::getSlot() const {
	return slot;
}