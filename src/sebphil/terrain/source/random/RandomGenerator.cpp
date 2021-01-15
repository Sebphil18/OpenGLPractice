#include "random/RandomGenerator.h"

RandomGenerator::RandomGenerator() {
	seed = randDevice();
	randEngine = std::mt19937(seed);
	intDistribution = std::uniform_int_distribution<int>();
	floatDistribution = std::uniform_real_distribution<float>();
}

RandomGenerator::RandomGenerator(int minInclusive, int maxInclusive) {
	seed = randDevice();
	randEngine = std::mt19937(seed);
	intDistribution = std::uniform_int_distribution<int>(minInclusive, maxInclusive);
	floatDistribution = std::uniform_real_distribution<float>(minInclusive, maxInclusive);
}

RandomGenerator::RandomGenerator(int minInclusive, int maxInclusive, int seed) {
	this->seed = seed;
	randEngine = std::mt19937(seed);
	intDistribution = std::uniform_int_distribution<int>(minInclusive, maxInclusive);
	floatDistribution = std::uniform_real_distribution<float>(minInclusive, maxInclusive);
}

void RandomGenerator::setSeed(int seed) {
	randEngine.seed(seed);
}

int RandomGenerator::getSeed() const {
	return seed;
}

int RandomGenerator::nextInt() {
	return intDistribution(randEngine);
}

float RandomGenerator::nextFloat() {
	return floatDistribution(randEngine);
}