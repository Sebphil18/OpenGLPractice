#pragma once
#include "heightmap/HeightMap.h"
#include <array>
#include "glm/glm.hpp"

#define SEB_KERNEL_SIZE 9

enum class EffectType {
	blur, sharpen
};

class SpecialEffect {
	
private:
	static const std::array<glm::vec2, SEB_KERNEL_SIZE> offsets;

	EffectType type;
	std::array<float, SEB_KERNEL_SIZE> kernel;

	std::array<float, SEB_KERNEL_SIZE> getKernelFor(EffectType type);
	std::array<float, SEB_KERNEL_SIZE> getBlurKernel();
	std::array<float, SEB_KERNEL_SIZE> getSharpenKernel();
	std::array<float, SEB_KERNEL_SIZE> getIdentityKernel();

	void applyEffect(std::vector<float>& newValues, const HeightMap& heightmap) const;
	std::array<float, SEB_KERNEL_SIZE> getSampledValues(glm::vec2 position, const HeightMap& heightmap) const;
	float samplePoint(glm::vec2 position, glm::vec2 offset, const HeightMap& heightmap) const;
	bool pointIsOutOfBounds(glm::vec2 point, const HeightMap& heightmap) const;
	float applyKernel(const std::array<float, SEB_KERNEL_SIZE>& sampledValues) const;

public:
	SpecialEffect(EffectType type);
	void apply(HeightMap& heightmap) const;

	EffectType getEfffectType() const;

};