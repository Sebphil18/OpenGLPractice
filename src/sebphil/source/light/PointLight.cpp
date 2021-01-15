#include "light/PointLight.h"
#include <string>

PointLight::PointLight() :
	index(0),
	constK(1),
	linearK(0.04f),
	quadraticK(0.032f),
	position(glm::vec3(0, 0, 0)),
	diffuseColor(glm::vec3(1, 1, 1)),
	specularColor(glm::vec3(0.8, 0.8, 0.8)),
	ambientColor(glm::vec3(0.1, 0.1, 0.1))
{}

PointLight::PointLight(std::size_t index):
	index(index),
	constK(1),
	linearK(0.04f),
	quadraticK(0.032f),
	position(glm::vec3(0, 0, 0)),
	diffuseColor(glm::vec3(1, 1, 1)),
	specularColor(glm::vec3(0.8, 0.8, 0.8)),
	ambientColor(glm::vec3(0.1, 0.1, 0.1)) 
{}

void PointLight::update(ShaderProgram& program) const {

	std::string uniName = "pointLights[" + std::to_string(index) + "].";

	updateK(program, uniName);
	updatePosition(program, uniName);
	updateColors(program, uniName);
}

void PointLight::updateK(ShaderProgram& program, const std::string& uniName) const {
	program.setUniform1f(uniName + "constantK", constK);
	program.setUniform1f(uniName + "linearK", linearK);
	program.setUniform1f(uniName + "quadraticK", quadraticK);
}

void PointLight::updatePosition(ShaderProgram& program, const std::string& uniName) const {
	program.setUniformVec3f(uniName + "position", position);
}

void PointLight::updateColors(ShaderProgram& program, const std::string& uniName) const {
	program.setUniformVec3f(uniName + "diffuseColor", diffuseColor);
	program.setUniformVec3f(uniName + "specularColor", specularColor);
	program.setUniformVec3f(uniName + "ambientColor", ambientColor);
}

bool PointLight::isEqualTo(const PointLight& pLight) const {
	return constK == pLight.constK
		&& linearK == pLight.linearK
		&& quadraticK == pLight.quadraticK
		&& position == pLight.position
		&& diffuseColor == pLight.diffuseColor
		&& specularColor == pLight.specularColor
		&& ambientColor == pLight.ambientColor;
}

void PointLight::setK(float constK, float linearK, float quadraticK) {
	this->constK = constK;
	this->linearK = linearK;
	this->quadraticK = quadraticK;
}

void PointLight::setPosition(glm::vec3 position) {
	this->position = position;
}

void PointLight::setDiffuseColor(glm::vec3 diffuseColor) {
	this->diffuseColor = diffuseColor;
}

void PointLight::setSpecularColor(glm::vec3 specularColor) {
	this->specularColor = specularColor;
}

void PointLight::setAmbientColor(glm::vec3 abmientColor) {
	this->ambientColor = ambientColor;
}

void PointLight::setIndex(std::size_t index) {
	this->index = index;
}

glm::vec3 PointLight::getPosition() const {
	return position;
}

glm::vec3 PointLight::getDiffuseColor() const {
	return diffuseColor;
}

glm::vec3 PointLight::getSpecularColor() const {
	return specularColor;
}

glm::vec3 PointLight::getAmbientColor() const {
	return ambientColor;
}