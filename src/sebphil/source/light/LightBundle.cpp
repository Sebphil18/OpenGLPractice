#include "light/LightBundle.h"

void LightBundle::update(ShaderProgram& program) {
	updateDirLights(program);
	updatePointLights(program);
}

void LightBundle::updateDirLights(ShaderProgram& program) {

	program.setUniform1i("dirLightsCount", dirLights.size());

	for (std::size_t i = 0; i < dirLights.size(); i++) {
		dirLights[i].setIndex(i);
		dirLights[i].update(program);
	}

}

void LightBundle::updatePointLights(ShaderProgram& program) {
			
	program.setUniform1i("pointLightsCount", pointLights.size());

	for (std::size_t i = 0; i < pointLights.size(); i++) {
		pointLights[i].setIndex(i);
		pointLights[i].update(program);
	}

}

void LightBundle::removeDirLight(std::size_t index) {
	dirLights.erase(dirLights.begin() + index);
}

void LightBundle::removeSameDirLights(const DirectionLight& dirLight) {

	for (auto it = dirLights.begin(); it != dirLights.end(); it++) {
		if (it->isEqualTo(dirLight))
			dirLights.erase(it);
	}

}

void LightBundle::removePointLight(std::size_t index) {
	pointLights.erase(pointLights.begin() + index);
}

void LightBundle::removeSamePointLights(const PointLight& pointLight) {

	for (auto it = pointLights.begin(); it != pointLights.end(); it++) {
		if (it->isEqualTo(pointLight))
			pointLights.erase(it);
	}

}

void LightBundle::clear() {
	dirLights.clear();
	pointLights.clear();
}

std::size_t LightBundle::getDirLightCount() const {
	return dirLights.size();
}

std::size_t LightBundle::getPointLightCount() const {
	return pointLights.size();
}