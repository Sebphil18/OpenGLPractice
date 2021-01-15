#pragma once
#include <cstdint>
#include <random>
#include "glm/glm.hpp"
#include "random/RandomGenerator.h"

template <int tablelength>
class PermutationTable {

private:
	int length;
	std::uint32_t indices[tablelength * 2];
	glm::vec2 objects[tablelength];
	RandomGenerator indexRandom;
	RandomGenerator vectorRandom;

	void fillTable();
	void permuteValues();

public:
	inline PermutationTable();
	inline PermutationTable(int seed);

	void permute();

	void setSeed(int seed);

	inline std::size_t getIndex(int x, int y);
	inline glm::vec2 getVector(std::size_t index);
	inline int getLength() const;
	inline int getSeed() const;

};

template<int tablelength>
inline PermutationTable<tablelength>::PermutationTable() :
	indexRandom(0, tablelength), vectorRandom(-1, 1), length(tablelength) {
	permute();
}

template<int tablelength>
inline PermutationTable<tablelength>::PermutationTable(int seed) :
	indexRandom(0, tablelength, seed), vectorRandom(-1, 1, seed), length(tablelength) {
	permute();
}

template<int tablelength>
inline void PermutationTable<tablelength>::setSeed(int seed) {
	indexRandom.setSeed(seed);
	vectorRandom.setSeed(seed);
	permute();
}

template<int tablelength>
void PermutationTable<tablelength>::permute() {
	fillTable();
	permuteValues();
}

template<int tablelength>
void PermutationTable<tablelength>::fillTable() {
	for (int i = 0; i < tablelength; i++) {
		indices[i] = i;
		float x = vectorRandom.nextFloat();
		float y = vectorRandom.nextFloat();
		objects[i] = glm::vec2(x, y);
	}
}

template<int tablelength>
void PermutationTable<tablelength>::permuteValues() {
	for (int i = 0; i < tablelength; i++) {
		std::size_t randIndex = indexRandom.nextInt();
		indices[i] = indices[randIndex];
		indices[i + tablelength] = indices[i];
	}
}

template<int tablelength>
inline std::size_t PermutationTable<tablelength>::getIndex(int x, int y) {
	return indices[indices[x] + y];
}

template<int tablelength>
inline glm::vec2 PermutationTable<tablelength>::getVector(std::size_t index) {
	return objects[index];
}

template<int tablelength>
inline int PermutationTable<tablelength>::getLength() const {
	return length;
}

template<int tablelength>
inline int PermutationTable<tablelength>::getSeed() const {
	return indexRandom.getSeed();
}