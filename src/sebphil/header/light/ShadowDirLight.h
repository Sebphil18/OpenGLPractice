#pragma once
#include "DirectionLight.h"
#include "modelstructure/Model.h"
#include <vector>

class ShadowDirLight : public DirectionLight {

private:
	uint32_t shadowMap;
	uint32_t shadowFbo;

	uint32_t shadowWidth;
	uint32_t shadowHeight;

	float left, right, top, bottom, near, far;

	glm::mat4 projMat;
	glm::mat4 viewMat;

	void initialize();

	void setUpTexture();
	void setUpTextureParam();
	void setUpFramebuffer();

	void updateViewMat();
	void updateProjMat();
	void updateLightSpaceMat(ShaderProgram& shadowProgram, ShaderProgram& program);
	
	void renderModels(const std::vector<std::shared_ptr<Model>>& models, ShaderProgram& shadowProgram);
	void bindFbo();
	void adjustViewport();
	void unbindFbo();

	void update(ShaderProgram& program) const override;

public:
	ShadowDirLight();
	ShadowDirLight(uint32_t shadowWidth, uint32_t shadowHeight);
	ShadowDirLight(std::size_t index);
	ShadowDirLight(std::size_t index, uint32_t shadowWidth, uint32_t shadowHeight);

	void update(const std::vector<std::shared_ptr<Model>>& models, ShaderProgram& program, ShaderProgram& shadowProgram);

	void setProjMat(glm::mat4 projMat);
	void setViewMat(glm::mat4 viewMat);
	void setDirection(glm::vec3 direction) override;

	uint32_t getShadowMapID() const;
	glm::mat4 getLightSpaceMat() const;

};
