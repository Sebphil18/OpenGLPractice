#pragma once
#include <random>

class RandomGenerator {

private:
	int seed;
	std::random_device randDevice;
	std::mt19937 randEngine;
	std::uniform_int_distribution<int> intDistribution;
	std::uniform_real_distribution<float> floatDistribution;

public:
	RandomGenerator();
	RandomGenerator(int minInclusive, int maxInclusive);
	RandomGenerator(int minInclusive, int maxInclusive, int seed);

	int nextInt();
	float nextFloat();

	void setSeed(int seed);
	int getSeed() const;

};
