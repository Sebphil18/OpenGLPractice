#include "erosion/Droplet.h"
#include <iostream>

Droplet::Droplet(float x, float y) {
	position = glm::vec2(x, y);
	prevPosition = glm::vec2(0);
	direction = glm::vec2(0);
	gradient = glm::vec2(0);

	velocity = 1;
	water = 1;
	sediment = 0;
	heightmap = nullptr;
}

// TODO: too long!
void Droplet::erode(HeightMap& heightmap, const DropletSettings& settings) {
	this->heightmap = &heightmap;

	for (std::uint32_t i = 0; i < settings.lifetime; i++) {

		HeightMapPoint heightPoint = getHeightMapPoint(position);

		prevPosition = position;
		float prevHeight = getHeight(position);

		setDirection(settings);
		position += direction;

		if (positionIsOutOfBounds(position) || isnan(direction.x) || isnan(direction.y))
			return;

		float newHeight = getHeight(position);
		float deltaHeight = newHeight - prevHeight;

		processSediment(deltaHeight, heightPoint, settings);
		updateVelocityAndWater(deltaHeight, settings);
	}

}

HeightMapPoint Droplet::getHeightMapPoint(glm::vec2 position) {
	HeightMapPoint heightPoint;
	heightPoint.point = glm::vec2(glm::floor(position.x), glm::floor(position.y));
	heightPoint.delta = glm::vec2(position.x - heightPoint.point.x, position.y - heightPoint.point.y);
	return heightPoint;
}

void Droplet::setDirection(const DropletSettings& settings) {
	// more mass -> more inertia
	float inertia = water * sediment * settings.inertiaMultiplier;

	direction.x = direction.x * inertia - gradient.x * (1.0f - inertia);
	direction.y = direction.y * inertia - gradient.y * (1.0f - inertia);
	direction = glm::normalize(direction);
}

bool Droplet::positionIsOutOfBounds(glm::vec2 position) {
	return position.x < 0 
		|| position.y < 0
		|| position.x > heightmap->getLengthX() - 2 
		|| position.y > heightmap->getLengthY() - 2;
}

float Droplet::getHeight(glm::vec2 position) {
	// This should be glm::vec2(glm::floor(position.x), glm::floor(position.y)), 
	// but this conversion is faster and will deliver the same result for POSITIVE POSITIONS!
	glm::vec2 point = glm::vec2((int)position.x, (int)position.y);
	glm::vec2 delta = glm::vec2(position.x - point.x, position.y - point.y);

	glm::vec4 cellHeights = getHeightValuesOfCell(point);

	// bilinear interpolation between height values
	gradient.x = (cellHeights.y - cellHeights.x) * (1 - delta.y) + (cellHeights.z - cellHeights.w) * delta.y;
	gradient.y = (cellHeights.z - cellHeights.x) * (1 - delta.x) + (cellHeights.w - cellHeights.y) * delta.x;

	return cellHeights.x * (1 - delta.x) * (1 - delta.y) 
		+ cellHeights.y * delta.x * (1 - delta.y) + cellHeights.z * (1 - delta.x) * delta.y 
		+ cellHeights.w * delta.x * delta.y;
}

glm::vec4 Droplet::getHeightValuesOfCell(glm::vec2 point) {
	float heightDownLeft = heightmap->getValue(point.x, point.y);
	float heightDownRight = heightmap->getValue(point.x + 1, point.y);
	float heightUpLeft = heightmap->getValue(point.x, point.y + 1);
	float heightUpRight = heightmap->getValue(point.x + 1, point.y + 1);

	return glm::vec4(heightDownLeft, heightDownRight, heightUpLeft, heightUpRight);
}

void Droplet::processSediment(float deltaHeight, const HeightMapPoint heightPoint, const DropletSettings& settings) {
	float capacity = glm::max(-deltaHeight, settings.minSlope) * water * velocity * settings.capacityMultiplier;

	if (deltaHeight > 0)
		dropRedundantSediment(deltaHeight, heightPoint);

	else if (sediment > capacity)
		dropSediment(capacity, heightPoint, settings);

	else if (capacity > sediment)
		pickupSediment(capacity, deltaHeight, settings);
}

void Droplet::dropRedundantSediment(float deltaHeight, const HeightMapPoint& heightPoint) {
	float deposit = glm::min(deltaHeight, sediment);
	depositSediment(deposit, heightPoint);
}

void Droplet::dropSediment(float capacity, const HeightMapPoint& heightPoint, const DropletSettings& settings) {
	float deposit = (sediment - capacity) * settings.depositRate;
	depositSediment(deposit, heightPoint);
}

void Droplet::depositSediment(float deposit, const HeightMapPoint& heightPoint) {
	sediment -= deposit;

	const glm::vec2& delta = heightPoint.delta;
	const glm::vec2& point = heightPoint.point;

	heightmap->setValue(point.x, point.y, heightmap->getValue(point.x, point.y) + deposit * (1 - delta.x) * (1 - delta.y));
	heightmap->setValue(point.x + 1, point.y, heightmap->getValue(point.x + 1, point.y) + deposit * delta.x * (1 - delta.y));
	heightmap->setValue(point.x, point.y + 1, heightmap->getValue(point.x, point.y + 1) + deposit * (1 - delta.x) * delta.y);
	heightmap->setValue(point.x + 1, point.y + 1, heightmap->getValue(point.x + 1, point.y + 1) + deposit * delta.x * delta.y);
}

void Droplet::pickupSediment(float capacity, float deltaHeight, const DropletSettings& settings) {
	float erode = glm::min(-deltaHeight, (capacity - sediment) * settings.erosionRate);
	brush(prevPosition, erode, settings.brushRadius);
}

void Droplet::brush(glm::vec2 position, float strength, float radius) {
	Bounds bounds = getBoundsOfBrush(position, radius);
	
	for (std::uint32_t quadX = bounds.minX; quadX <= bounds.maxX; quadX++) {
		for (std::uint32_t quadY = bounds.minY; quadY <= bounds.maxY; quadY++) {

			glm::vec2 delta(position.x - quadX, position.y - quadY);

			if (pointIsInCircle(delta, radius)) {
				float value = getErosionValue(delta, strength, radius);
				applyErosionValue(value, glm::vec2(quadX, quadY));
				sediment += value;
			}
		}
	}
}

Bounds Droplet::getBoundsOfBrush(glm::vec2 position, float radius) {
	Bounds bounds;
	bounds.minX = static_cast<int>(position.x - radius);
	bounds.minY = static_cast<int>(position.y - radius);
	bounds.maxX = static_cast<int>(position.x + radius);
	bounds.maxY = static_cast<int>(position.y + radius);

	std::uint32_t maxIndexX = heightmap->getLengthX() - 1;
	std::uint32_t maxIndexY = heightmap->getLengthY() - 1;
	bounds.minX = bounds.minX < 0 ? 0 : bounds.minX;
	bounds.minY = bounds.minY < 0 ? 0 : bounds.minY;
	bounds.maxX = bounds.maxX > maxIndexX ? maxIndexX : bounds.maxX;
	bounds.maxY = bounds.maxY > maxIndexY ? maxIndexY : bounds.maxY;

	return bounds;
}

bool Droplet::pointIsInCircle(glm::vec2 delta, float radius) {
	return delta.x * delta.x + delta.y * delta.y <= radius * radius;
}

float Droplet::getErosionValue(glm::vec2 delta, float strength, float radius) {
	float length = glm::sqrt(delta.x * delta.x + delta.y * delta.y) / radius;
	return (1.0f - length) * strength;
}

void Droplet::applyErosionValue(float value, glm::vec2 quadPosition) {
	float newHeight = heightmap->getValue(quadPosition.x, quadPosition.y) - value;
	heightmap->setValue(quadPosition.x, quadPosition.y, newHeight);
}

void Droplet::updateVelocityAndWater(float deltaHeight, const DropletSettings& settings) {
	velocity = glm::sqrt(velocity * velocity + deltaHeight * settings.gravity);
	water *= (1 - settings.evaporationRate);
}