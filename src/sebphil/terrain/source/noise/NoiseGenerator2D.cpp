#include "noise/NoiseGenerator2D.h"

NoiseGenerator2D::NoiseGenerator2D() :
	frequency(1) {}

float NoiseGenerator2D::noise(float x, float y) {
	x *= frequency;
	y *= frequency;

	GridCell cell;
	cell.floorX = std::floor(x);
	cell.floorY = std::floor(y);
	cell.deltaX = x - cell.floorX;
	cell.deltaY = y - cell.floorY;
	getGradients(cell);
	calculateCenterVectors(cell);
	calculateCenterVectors(cell);
	calculateDotProducts(cell);
	return interpolateXY(cell);
}

void NoiseGenerator2D::getGradients(GridCell& cell) {
	std::size_t minX = static_cast<int>(cell.floorX) % permutation.getLength();
	std::size_t minY = static_cast<int>(cell.floorY) % permutation.getLength();
	std::size_t maxX = static_cast<int>(minX + 1) % permutation.getLength();
	std::size_t maxY = static_cast<int>(minY + 1) % permutation.getLength();
	cell.gradients[0] = permutation.getVector(permutation.getIndex(minX, minY));
	cell.gradients[1] = permutation.getVector(permutation.getIndex(maxX, minY));
	cell.gradients[2] = permutation.getVector(permutation.getIndex(minX, maxY));
	cell.gradients[3] = permutation.getVector(permutation.getIndex(maxX, maxY));
}

void NoiseGenerator2D::calculateCenterVectors(GridCell& cell) {
	float x2 = cell.deltaX - 1.0f;
	float y2 = cell.deltaY - 1.0f;
	cell.centerVectors[0] = glm::vec2(cell.deltaX, cell.deltaY);
	cell.centerVectors[1] = glm::vec2(x2, cell.deltaY);
	cell.centerVectors[2] = glm::vec2(cell.deltaX, y2);
	cell.centerVectors[3] = glm::vec2(x2, y2);
}

void NoiseGenerator2D::calculateDotProducts(GridCell& cell) {
	for (std::uint32_t i = 0; i < 4; i++)
		cell.dotProducts[i] = glm::dot(cell.gradients[i], cell.centerVectors[i]);
}

float NoiseGenerator2D::interpolateXY(GridCell& cell) {
	float smoothX = smoothStep(cell.deltaX);
	float smoothY = smoothStep(cell.deltaY);
	float interpolX1 = interpolate(cell.dotProducts[0], cell.dotProducts[1], smoothX);
	float interpolX2 = interpolate(cell.dotProducts[2], cell.dotProducts[3], smoothX);
	return interpolate(interpolX1, interpolX2, smoothY);
}

inline float NoiseGenerator2D::smoothStep(float value) {
	return value * value * (3.0f - 2.0f * value);
}

inline float NoiseGenerator2D::interpolate(float x, float y, float a) {
	return x + a * (y - x);
}

void NoiseGenerator2D::setFrequency(float frequency) {
	this->frequency = frequency;
}

void NoiseGenerator2D::setSeed(std::uint32_t seed) {
	permutation.setSeed(seed);
}

int NoiseGenerator2D::getSeed() const {
	return permutation.getSeed();
}

float NoiseGenerator2D::getFrequency() const {
	return frequency;
}

float* const NoiseGenerator2D::getFrequencyPtr() {
	return &frequency;
}