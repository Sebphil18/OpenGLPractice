#pragma once
#include <cstddef>
#include <vector>

enum class UniformType { integer, decimal, vec2D, vec3D, vec4D, matrix4 };

struct UniformBufferElement {
	unsigned int size;
	unsigned int offset;
	const void* data;
};

class UniformBuffer {

private:
	
	unsigned int glID;
	unsigned int length;
	std::size_t slot;
	std::size_t size;
	std::vector<UniformBufferElement> elements;

	static std::size_t count;

	void bindBufferToSlot();

	void fillSizes(unsigned int length);
	void setSlot();
	void generateBuffer();
	void setData(std::size_t size);
	void deleteBuffer();
	void setElementSize(UniformBufferElement& element, UniformType typeOfElement);
	unsigned int getSizeOfType(UniformType type);
	unsigned int getOffsetToElement(std::size_t indexOfElement);
	void setSubData(const UniformBufferElement& element);

public:

	UniformBuffer(unsigned int length, std::size_t size);
	~UniformBuffer();

	void bind();
	void unbind();

	void setElementType(std::size_t index, UniformType type);
	void setElementData(std::size_t index, const void* data);

	unsigned int getID() const;
	std::size_t getSlot() const;

};

