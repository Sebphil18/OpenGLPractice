#include "erosion/HydraulicErosion.h"

HydraulicErosion::HydraulicErosion(std::uint32_t iterations):
	iterations(iterations) {
    dropSettings.lifetime = 40;
    dropSettings.minSlope = 0.005;
    dropSettings.capacityMultiplier = 2.2;
    dropSettings.inertiaMultiplier = 0.2;
    dropSettings.depositRate = 0.1;
    dropSettings.erosionRate = 0.13;
    dropSettings.gravity = 6;
    dropSettings.evaporationRate = 0.01;
    dropSettings.brushRadius = 5;
}

void HydraulicErosion::erode(HeightMap& heightMap) {

    RandomGenerator randomX(0, heightMap.getLengthX() - 2);
    RandomGenerator randomY(0, heightMap.getLengthY() - 2);

    for (int i = 0; i < iterations; i++) {
        float x = randomX.nextFloat();
        float y = randomY.nextFloat();

        Droplet drop(x, y);
        drop.erode(heightMap, dropSettings);
    }
}

int* HydraulicErosion::getIterationsPtr() {
    return &iterations;
}

DropletSettings* HydraulicErosion::getDropletSettingsPtr() {
    return &dropSettings;
}