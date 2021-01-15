#include "heightmap/SpecialEffect.h"
#include <vector>

const std::array<glm::vec2, SEB_KERNEL_SIZE> SpecialEffect::offsets = {
		glm::vec2(-1, 1), glm::vec2(0, 1), glm::vec2(1, 1),
		glm::vec2(-1, 0), glm::vec2(0, 0), glm::vec2(1, 0),
		glm::vec2(-1, -1), glm::vec2(0, -1), glm::vec2(1, -1)
};

SpecialEffect::SpecialEffect(EffectType type) :
	type(type) {
	kernel = getKernelFor(type);
}

std::array<float, SEB_KERNEL_SIZE> SpecialEffect::getKernelFor(EffectType type) {

	std::array<float, 9> kernel;

	switch (type) {
		case EffectType::blur:
			return getBlurKernel();
		case EffectType::sharpen:
			return getSharpenKernel();
		default:
			return getIdentityKernel();
	};
}

std::array<float, SEB_KERNEL_SIZE> SpecialEffect::getBlurKernel() {
	return {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};
}

std::array<float, SEB_KERNEL_SIZE> SpecialEffect::getSharpenKernel() {
	return {
		0, -1, 0,
		-1, 5, -1,
		0, -1, 0
	};
}

std::array<float, SEB_KERNEL_SIZE> SpecialEffect::getIdentityKernel() {
	return {
		0, 0, 0,
		0, 1, 0,
		0, 0, 0
	};
}

void SpecialEffect::apply(HeightMap& heightmap) const {
	int lengthX = heightmap.getLengthX();
	int lengthY = heightmap.getLengthY();
	std::vector<float> combinedValues(static_cast<std::size_t>(lengthX) * lengthY);

	applyEffect(combinedValues, heightmap);

	heightmap.setValues(lengthX, lengthY, combinedValues);
}

void SpecialEffect::applyEffect(std::vector<float>& newValues, const HeightMap& heightmap) const {
	for (int x = 0; x < heightmap.getLengthX(); x++) {
		for (int y = 0; y < heightmap.getLengthY(); y++) {

			std::array<float, SEB_KERNEL_SIZE> sampledValues = getSampledValues(glm::vec2(x, y), heightmap);
			float combinedValue = applyKernel(sampledValues);

			std::size_t index = static_cast<std::size_t>(y) * heightmap.getLengthX() + x;
			newValues[index] = combinedValue;
		}
	}
}

std::array<float, SEB_KERNEL_SIZE> SpecialEffect::getSampledValues(glm::vec2 position, const HeightMap& heightmap) const {

	std::array<float, SEB_KERNEL_SIZE> sampledValues;

	for (int i = 0; i < SEB_KERNEL_SIZE; i++)
		sampledValues[i] = samplePoint(position, offsets[i], heightmap);

	return sampledValues;
}

float SpecialEffect::samplePoint(glm::vec2 position, glm::vec2 offset, const HeightMap& heightmap) const {
	
	glm::vec2 samplePoint = position + offset;

	if (pointIsOutOfBounds(samplePoint, heightmap)) {
		return heightmap.getValue(position.x, position.y);
	}

	return heightmap.getValue(samplePoint.x, samplePoint.y);
}

bool SpecialEffect::pointIsOutOfBounds(glm::vec2 point, const HeightMap& heightmap) const {
	return point.x < 0 || point.x > heightmap.getLengthX() - 1
		|| point.y < 0 || point.y > heightmap.getLengthY() - 1;
}

float SpecialEffect::applyKernel(const std::array<float, SEB_KERNEL_SIZE>& sampledValues) const {

	float combinedValue = 0;
	for (int i = 0; i < SEB_KERNEL_SIZE; i++)
		combinedValue += sampledValues[i] * kernel[i];

	return combinedValue;
}

EffectType SpecialEffect::getEfffectType() const {
	return type;
}