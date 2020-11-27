#pragma once
#include "shader/ShaderProgram.h"
#include "glm/glm.hpp"

class DirectionLight {

private:

	std::size_t index;

	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	glm::vec3 ambientColor;

protected:

	glm::vec3 direction;

public:

	DirectionLight();
	DirectionLight(std::size_t index);

	virtual void update(ShaderProgram& program) const;

	bool isEqualTo(const DirectionLight& dirLight) const;

	void setIndex(std::size_t index);
	virtual void setDirection(glm::vec3 direction);
	void setDiffuseColor(glm::vec3 diffuseColor);
	void setSpecularColor(glm::vec3 specularColor);
	void setAmbientColor(glm::vec3 ambientColor);

	glm::vec3 getDirection() const;
	glm::vec3 getDiffuseColor() const;
	glm::vec3 getSpecularColor() const;
	glm::vec3 getAmbientColor() const;
	std::size_t getIndex() const;

};