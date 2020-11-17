#include "header/DirectionLight.h"
#include <string>

DirectionLight::DirectionLight():
	index(0),
	direction(glm::vec3(0.5, -0.5, 0)),
	diffuseColor(glm::vec3(1, 1, 1)),
	specularColor(glm::vec3(0.8, 0.8, 0.8)),
	ambientColor(glm::vec3(0.1, 0.1, 0.1))
{}

DirectionLight::DirectionLight(std::size_t index) :
	index(index),
	direction(glm::vec3(0.5, -0.5, 0)),
	diffuseColor(glm::vec3(1, 1, 1)),
	specularColor(glm::vec3(0.8, 0.8, 0.8)),
	ambientColor(glm::vec3(0.1, 0.1, 0.1)) 
{}

void DirectionLight::update(ShaderProgram& program) const {

	std::string uniPrefix = "dirLights[" + std::to_string(index) + "].";

	program.setUniformVec3f(uniPrefix + "direction", direction);
	program.setUniformVec3f(uniPrefix + "diffuseColor", diffuseColor);
	program.setUniformVec3f(uniPrefix + "specularColor", specularColor);
	program.setUniformVec3f(uniPrefix + "ambientColor", ambientColor);

}

bool DirectionLight::isEqualTo(const DirectionLight& dirLight) const {
	return direction == dirLight.getDirection()
		&& diffuseColor == dirLight.getDiffuseColor()
		&& specularColor == dirLight.getSpecularColor()
		&& ambientColor == dirLight.getAmbientColor();
}

void DirectionLight::setIndex(std::size_t index) {
	this->index = index;
}

void DirectionLight::setDirection(glm::vec3 direction) {
	this->direction = direction;
}

void DirectionLight::setDiffuseColor(glm::vec3 diffuseColor) {
	this->diffuseColor = diffuseColor;
}

void DirectionLight::setSpecularColor(glm::vec3 specularColor) {
	this->specularColor = specularColor;
}

void DirectionLight::setAmbientColor(glm::vec3 ambientColor) {
	this->ambientColor = ambientColor;
}

glm::vec3 DirectionLight::getDirection() const {
	return direction;
}

glm::vec3 DirectionLight::getDiffuseColor() const {
	return diffuseColor;
}

glm::vec3 DirectionLight::getSpecularColor() const {
	return specularColor;
}

glm::vec3 DirectionLight::getAmbientColor() const {
	return ambientColor;
}

std::size_t DirectionLight::getIndex() const {
	return index;
}