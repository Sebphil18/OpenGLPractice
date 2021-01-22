#include "erosion/HydraulicErosion.h"

HydraulicErosion::HydraulicErosion(std::uint32_t iterations):
	amountOfDrops(iterations) {
    dropSettings.lifetime = 40;
    dropSettings.minSlope = 0.15;
    dropSettings.capacityMultiplier = 19;
    dropSettings.inertiaMultiplier = 0.2;
    dropSettings.depositRate = 0.15;
    dropSettings.erosionRate = 0.85;
    dropSettings.gravity = 0.13;
    dropSettings.evaporationRate = 0.01;
    dropSettings.brushRadius = 1.48;
}

void HydraulicErosion::erode(HeightMap& heightMap) {

    RandomGenerator randomX(0, heightMap.getLengthX() - 2);
    RandomGenerator randomY(0, heightMap.getLengthY() - 2);

    for (int i = 0; i < amountOfDrops; i++) {
        float x = randomX.nextFloat();
        float y = randomY.nextFloat();

        Droplet drop(x, y);
        drop.erode(heightMap, dropSettings);
    }
}

int* HydraulicErosion::getDropAmountPtr() {
    return &amountOfDrops;
}

DropletSettings* HydraulicErosion::getDropletSettingsPtr() {
    return &dropSettings;
}