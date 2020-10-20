#pragma once
#include "header/DirectionLight.h"

class ShadowDirLight : public DirectionLight {

private:
	uint32_t shadowMap;
	uint32_t shadowFbo;

	uint32_t shadowWidth;
	uint32_t shadowHeight;

	int left, right, top, bottom, near, far;

	glm::mat4 projMat;
	glm::mat4 viewMat;

	void initialize();

	void setUpTexture();
	void setUpTextureParam();
	void setUpFramebuffer();

public:
	ShadowDirLight();
	ShadowDirLight(uint32_t shadowWidth, uint32_t shadowHeight);
	ShadowDirLight(std::size_t index);
	ShadowDirLight(std::size_t index, uint32_t shadowWidth, uint32_t shadowHeight);

	void bindFbo();

	void update(ShaderProgram& program) const override;
	void updateProjMat();
	void updateViewMat();

	void setViewport();
	void setProjMat(glm::mat4 projMat);
	void setViewMat(glm::mat4 viewMat);

	uint32_t getShadowMapID() const;
	glm::mat4 getLightSpaceMat() const;

};
