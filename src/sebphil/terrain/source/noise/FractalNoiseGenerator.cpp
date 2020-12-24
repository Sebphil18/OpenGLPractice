#include "noise/FractalNoiseGenerator.h"
#include <iostream>

FractalNoiseGenerator::FractalNoiseGenerator():
	amplitude(1), octaves(5), amplMultiplier(0.5), freqMultiplier(1.5), offsetX(420), offsetY(420) {}

float FractalNoiseGenerator::fractalNoise(float x, float y) {

	float sum = 0;
	float amplitude = this->amplitude;
	float frequency = this->getFrequency();

	for (int i = 0; i < octaves; i++) {
		sum += noise(x * frequency + offsetX, y * frequency + offsetY) * amplitude;
		amplitude *= amplMultiplier;
		frequency *= freqMultiplier;
	}

	return sum;
}

void FractalNoiseGenerator::setOcataves(int octaves) {
	this->octaves = octaves;
}

void FractalNoiseGenerator::setAmplitude(float amplitude) {
	this->amplitude = amplitude;
}

void FractalNoiseGenerator::setAmpMultiplier(float amplMultiplier) {
	this->amplMultiplier = amplMultiplier;
}

int* const FractalNoiseGenerator::getOctavesPtr() {
	return &octaves;
}

float* const FractalNoiseGenerator::getAmplitudePtr() {
	return &amplitude;
}

float* const FractalNoiseGenerator::getAmplMultiplierPtr() {
	return &amplMultiplier;
}

float* const FractalNoiseGenerator::getFreqMultiplierPtr() {
	return &freqMultiplier;
}