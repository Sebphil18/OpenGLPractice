#pragma once
#include "DirectionLight.h"
#include "PointLight.h"
#include "shader/ShaderProgram.h"
#include <vector>
#include <queue>

struct LightBundle {

private:

	void updateDirLights(ShaderProgram& program);
	void updatePointLights(ShaderProgram& program);

public:

	std::vector<DirectionLight> dirLights;
	std::vector<PointLight> pointLights;

	void update(ShaderProgram& program);

	void removeDirLight(std::size_t index);
	void removeSameDirLights(const DirectionLight& dirLight);

	void removePointLight(std::size_t index);
	void removeSamePointLights(const PointLight& pointLight);

	void clear();

	std::size_t getPointLightCount() const;
	std::size_t getDirLightCount() const;

};