#pragma once
#include <vector>

struct LayoutElement {
	unsigned int count;
	unsigned int type;
	unsigned int normalized;
};

class VertexBufferLayout {

private:

	unsigned int stride;
	std::vector<LayoutElement> elements;

	unsigned int getSizeOfType(unsigned int type);

	void addElementToStride(const LayoutElement& element);
	void removeElementFromStride(const LayoutElement& element);

	void addVertexAttribute(const LayoutElement& element, unsigned int offset, std::size_t index);
	void defineVertexAttribute(const LayoutElement& element, unsigned int offset, std::size_t index);
	void enableVertexAttribute(std::size_t index);

	unsigned int getAttributeOffset(const LayoutElement& element);

public:

	VertexBufferLayout();

	std::size_t addElement(const LayoutElement& element);
	void removeElement(std::size_t index);

	void bindLayout();

};

