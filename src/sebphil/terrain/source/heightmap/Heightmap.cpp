#include "heightmap/HeightMap.h"

HeightMap::HeightMap(int lengthX, int lengthY) {
	this->lengthX = lengthX;
	this->lengthY = lengthY;
	values = std::vector<float>((std::size_t) lengthX * lengthY, 0);
}

HeightMap::HeightMap(int lengthX, int lengthY, const std::vector<float>& values) {
	this->lengthX = lengthX;
	this->lengthY = lengthY;
	this->values = values;
}

void HeightMap::normalize() {
	float maxValue = -INFINITY;

	for (int i = 0; i < values.size(); i++) {
		if (values[i] > maxValue)
			maxValue = values[i];
	}

	for (int i = 0; i < values.size(); i++)
		values[i] = (values[i] + maxValue) / (maxValue * 2);
}

void HeightMap::clear() {
	lengthX = 0;
	lengthY = 0;
	values.clear();
}

void HeightMap::resize(int newLengthX, int newLengthY) {
	std::vector<float> newValues((std::size_t) newLengthX * newLengthY, 0);
	copyValues(newLengthX, newLengthY, newValues);

	this->lengthX = newLengthX;
	this->lengthY = newLengthY;
	values = newValues;
}

void HeightMap::copyValues(int lengthX, int lengthY, std::vector<float>& newValues) {
	for (int x = 0; x < std::min(lengthX, this->lengthX); x++) {
		for (int y = 0; y < std::min(lengthY, this->lengthY); y++) {

			std::size_t newIndex = (std::size_t)y * lengthX + x;
			std::size_t oldIndex = (std::size_t)y * this->lengthX + x;
			if (newIndex < values.size())
				newValues[newIndex] = values[oldIndex];

		}
	}
}

void HeightMap::setValue(int x, int y, float value) {
	std::size_t index = (std::size_t)y * lengthX + x;
	values[index] = value;
}

void HeightMap::setValues(int lengthX, int lengthY, const std::vector<float>& values) {
	this->lengthX = lengthX;
	this->lengthY = lengthY;
	this->values = values;
}

float HeightMap::getValue(int x, int y) const {
	std::size_t index = (std::size_t)y * lengthX + x;
	return values[index];
}

int HeightMap::getLengthX() const {
	return lengthX;
}

int HeightMap::getLengthY() const {
	return lengthY;
}