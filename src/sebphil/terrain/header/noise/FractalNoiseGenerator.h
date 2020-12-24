#pragma once
#include "NoiseGenerator2D.h"

class FractalNoiseGenerator : public NoiseGenerator2D {
private:
	int octaves;
	float amplitude;
	float freqMultiplier;
	float amplMultiplier;
	float offsetX, offsetY;

public:
	FractalNoiseGenerator();

	float fractalNoise(float x, float y);

	void setOcataves(int octaves);
	void setAmplitude(float amplitude);
	void setAmpMultiplier(float amplMultiplier);

	int* const getOctavesPtr();
	float* const getAmplitudePtr();
	float* const getAmplMultiplierPtr();
	float* const getFreqMultiplierPtr();

};