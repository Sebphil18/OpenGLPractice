#include "noise/FractalNoiseGenerator.h"
#include <iostream>

FractalNoiseGenerator::FractalNoiseGenerator():
	amplitude(23), octaves(10), amplMultiplier(0.57), freqMultiplier(1.6), warpMultiplier(0), 
	offsetX(420), offsetY(420) {}

float FractalNoiseGenerator::fractalNoise(float x, float y) {
	
	glm::vec2 offset = warp(x, y);

	return getNoiseValue(x + offset.x, y + offset.y);
}

glm::vec2 FractalNoiseGenerator::warp(float x, float y) {

	glm::vec2 offset(0);

	if (warpMultiplier != 0) {
		offset.x = glm::abs(getNoiseValue(x, y));
		offset.y = glm::abs(getNoiseValue(x + 10.432, y));
	}

	return offset * warpMultiplier;
}

float FractalNoiseGenerator::getNoiseValue(float x, float y) {

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

float* const FractalNoiseGenerator::getWarpMultiplierPtr() {
	return &warpMultiplier;
}