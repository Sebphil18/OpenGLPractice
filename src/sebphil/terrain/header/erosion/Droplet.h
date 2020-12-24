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

// One HeightPoint represents one point on the heightmap
struct HeightMapPoint {
	glm::vec2 point;
	glm::vec2 delta;
};

class Droplet {
private:
	float velocity;
	float water;
	float sediment;

	glm::vec2 position;
	glm::vec2 prevPosition;
	glm::vec2 direction;
	glm::vec2 gradient;

	HeightMapPoint getHeightMapPoint(glm::vec2 position);
	void setDirection(const DropletSettings& settings);
	bool positionIsOutOfBounds(glm::vec2 position, const HeightMap& heightmap);
	float getHeight(glm::vec2 position, const HeightMap& heightmap);
	void processSediment(const HeightMapPoint heightPoint, float deltaHeight, HeightMap& heightmap, const DropletSettings& settings);
	void depositSediment(float deposit, const HeightMapPoint& heightPoint, HeightMap& heightmap);
	void brush(glm::vec2 position, float strength, float radius, HeightMap& heightmap);
	Bounds getBoundsOfBrush(glm::vec2 position, float radius, const HeightMap& heightmap);
	bool pointIsInCircle(glm::vec2 delta, float radius);
	void updateVelocityAndWater(float deltaHeight, const DropletSettings& settings);

public:
	Droplet(float x, float y);
	void erode(HeightMap& heightmap, const DropletSettings& settings);

};
