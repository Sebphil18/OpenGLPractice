#pragma once
#include "heightmap/HeightMap.h"
#include "glm/glm.hpp"

struct DropletSettings {
	int lifetime; // defines maximum lifetime in ticks for a drop 
	float minSlope; // defines how eroded a flat region should be (higher means a flat area will have more texture)
	float capacityMultiplier; // scales the capacity of a drop in addition to its velocity, water etc.
	float inertiaMultiplier; // scales the inertia of a drop in addition to its current sediment and water
	float depositRate, erosionRate, evaporationRate;
	float gravity;
	float brushRadius; // defines how large a drop should be (radius = 1 means it only affects the current point of the heightmap)
};

struct Bounds {
	std::uint32_t minX, maxX;
	std::uint32_t minY, maxY;
};

// One HeightPoint represents one point on the heightmap, which can be located in the middle of a cell.
// So every point has stored its cell-coordinates (integer values) and an offset into the "middle" of the cell.
struct HeightMapPoint {
	glm::vec2 point;
	glm::vec2 delta;
};

// TODO: Outsource brush as pencil class on heightmap
class Droplet {

private:
	float velocity;
	float water;
	float sediment;

	glm::vec2 position;
	glm::vec2 prevPosition;
	glm::vec2 direction;
	glm::vec2 gradient;

	HeightMap* heightmap;

	HeightMapPoint getHeightMapPoint(glm::vec2 position);
	void setDirection(const DropletSettings& settings);
	bool positionIsOutOfBounds(glm::vec2 position);
	float getHeight(glm::vec2 position);
	glm::vec4 getHeightValuesOfCell(glm::vec2 point);

	void processSediment(float deltaHeight, const HeightMapPoint heightPoint, const DropletSettings& settings);
	
	void dropRedundantSediment(float deltaHeight, const HeightMapPoint& heightpoint);
	void dropSediment(float capacity, const HeightMapPoint& heightPoint, const DropletSettings& settings);
	void depositSediment(float deposit, const HeightMapPoint& heightPoint);

	void pickupSediment(float capacity, float deltaHeight, const DropletSettings& settings);

	void brush(glm::vec2 position, float strength, float radius);
	Bounds getBoundsOfBrush(glm::vec2 position, float radius);
	bool pointIsInCircle(glm::vec2 delta, float radius);
	float getErosionValue(glm::vec2 delta, float strength, float radius);
	void applyErosionValue(float value, glm::vec2 quadPosition);

	void updateVelocityAndWater(float deltaHeight, const DropletSettings& settings);

public:
	Droplet(float x, float y);
	void erode(HeightMap& heightmap, const DropletSettings& settings);

};
