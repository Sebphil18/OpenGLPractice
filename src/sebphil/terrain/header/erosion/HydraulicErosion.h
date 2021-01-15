#pragma once
#include <cinttypes>
#include "Droplet.h"
#include "random/RandomGenerator.h"

class HydraulicErosion {

private:

	int amountOfDrops;
	DropletSettings dropSettings;

public:
	HydraulicErosion(std::uint32_t iterations);
	void erode(HeightMap& heightMap);

	int* getDropAmountPtr();
	DropletSettings* getDropletSettingsPtr();

};