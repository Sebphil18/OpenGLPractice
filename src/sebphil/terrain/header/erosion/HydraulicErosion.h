#pragma once
#include <cinttypes>
#include "Droplet.h"
#include "random/RandomGenerator.h"

class HydraulicErosion {

private:
	int iterations;
	DropletSettings dropSettings;

public:
	HydraulicErosion(std::uint32_t iterations);
	void erode(HeightMap& heightMap);

	int* getIterationsPtr();
	DropletSettings* getDropletSettingsPtr();

};