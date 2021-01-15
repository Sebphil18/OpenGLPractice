#pragma once
#include <cstdint>
#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"
#include "PermutationTable.h"

#ifndef SEB_PERLIN_TABLELENGTH
#define SEB_PERLIN_TABLELENGTH 255
#endif

struct GridCell {
	float deltaX, deltaY;
	float floorX, floorY;
	glm::vec2 gradients[4];
	glm::vec2 centerVectors[4];
	float dotProducts[4];
};

class NoiseGenerator2D {

private:
	PermutationTable<SEB_PERLIN_TABLELENGTH> permutation;
	float frequency;
	
	void getGradients(GridCell& cell);
	void calculateCenterVectors(GridCell& cell);
	void calculateDotProducts(GridCell& cell);

	float interpolateXY(GridCell& cell);
	inline float smoothStep(float value);
	inline float interpolate(float x, float y, float a);

public:
	NoiseGenerator2D();
	//always returns value between 0 and 1
	float noise(float x, float y);

	void setSeed(std::uint32_t seed);
	void setFrequency(float frequency);

	int getSeed() const;
	float getFrequency() const;
	float* const getFrequencyPtr();

};
