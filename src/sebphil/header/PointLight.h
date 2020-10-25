#pragma once
#include "header/ShaderProgram.h"
#include "glm/glm.hpp"

class PointLight {

private:

	std::size_t index;

	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	glm::vec3 ambientColor;

protected:

	glm::vec3 position;
	float constK, linearK, quadraticK;

	void updateK(ShaderProgram& program, const std::string& uniName) const;
	void updatePosition(ShaderProgram& program, const std::string& uniName) const;
	void updateColors(ShaderProgram& program, const std::string& uniName) const;

public:

	PointLight();
	PointLight(std::size_t index);

	virtual void update(ShaderProgram& program) const;

	bool isEqualTo(const PointLight& pLight) const;

	void setK(float constK, float linearK, float quadraticK);
	virtual void setPosition(glm::vec3 position);
	void setDiffuseColor(glm::vec3 diffuseColor);
	void setSpecularColor(glm::vec3 specularColor);
	void setAmbientColor(glm::vec3 ambientColor);
	void setIndex(std::size_t index);

	glm::vec3 getPosition() const;
	glm::vec3 getDiffuseColor() const;
	glm::vec3 getSpecularColor() const;
	glm::vec3 getAmbientColor() const;

};