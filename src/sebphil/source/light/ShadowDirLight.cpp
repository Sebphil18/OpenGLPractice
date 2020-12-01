#include "light/ShadowDirLight.h"
#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"

ShadowDirLight::ShadowDirLight() :
	shadowWidth(4096), shadowHeight(4096),
	left(-20), right(20), bottom(-20), top(20), near(-20), far(20) {

	initialize();

}

ShadowDirLight::ShadowDirLight(uint32_t shadowWidth, uint32_t shadowHeight) :
	shadowWidth(shadowWidth), shadowHeight(shadowHeight),
	left(-20), right(20), bottom(-20), top(20), near(-20), far(20) {

	initialize();
}

ShadowDirLight::ShadowDirLight(std::size_t index) :
	shadowWidth(4096), shadowHeight(4096),
	left(-20), right(20), bottom(-20), top(20), near(-20), far(20),
	DirectionLight(index) {

	initialize();

}

ShadowDirLight::ShadowDirLight(std::size_t index, uint32_t shadowWidth, uint32_t shadowHeight):
	shadowWidth(shadowWidth), shadowHeight(shadowHeight),
	left(-20), right(20), bottom(-20), top(20), near(-20), far(20),
	DirectionLight(index) {

	initialize();

}

void ShadowDirLight::initialize() {

	setUpTexture();
	setUpFramebuffer();

	updateProjMat();
	updateViewMat();

}

void ShadowDirLight::setUpTexture() {
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	setUpTextureParam();
}

void ShadowDirLight::setUpTextureParam() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void ShadowDirLight::setUpFramebuffer() {
	glGenFramebuffers(1, &shadowFbo);
	bindFbo();
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowDirLight::update(const std::vector<Model*>& models, ShaderProgram& shadowProgram, ShaderProgram& program) {
	
	updateProjMat();
	updateViewMat();
	updateLightSpaceMat(shadowProgram, program);

	renderModels(models, shadowProgram);

	update(program);

}

void ShadowDirLight::updateProjMat() {
	projMat = glm::ortho(left, right, bottom, top, near, far);
}

void ShadowDirLight::updateViewMat() {
	viewMat = glm::lookAt(-direction, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void ShadowDirLight::updateLightSpaceMat(ShaderProgram& shadowProgram, ShaderProgram& program) {
	glm::mat4 lightSpaceMat = getLightSpaceMat();
	shadowProgram.setUniformMat4f("dirLightSpaceMat", lightSpaceMat);
	program.setUniformMat4f("dirLightSpaceMat", lightSpaceMat);
}

glm::mat4 ShadowDirLight::getLightSpaceMat() const {
	return projMat * viewMat;
}

void ShadowDirLight::renderModels(const std::vector<Model*> models, ShaderProgram& shadowProgram) {

	bindFbo();
	adjustViewport();

	for (std::size_t i = 0; i < models.size(); i++) {
		Model* model = models[i];
		shadowProgram.setUniformMat4f("worldMatrix", model->getWorldMat());
		model->draw(shadowProgram);
	}

	unbindFbo();

}

void ShadowDirLight::bindFbo() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFbo);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowDirLight::adjustViewport() {
	glViewport(0, 0, shadowWidth, shadowHeight);
}

void ShadowDirLight::unbindFbo() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowDirLight::update(ShaderProgram& program) const {

	std::string uniPrefix = "shadowDirLights[" + std::to_string(0) + "].";

	program.setUniformVec3f(uniPrefix + "direction", getDirection());
	program.setUniformVec3f(uniPrefix + "diffuseColor", getDiffuseColor());
	program.setUniformVec3f(uniPrefix + "specularColor", getSpecularColor());
	program.setUniformVec3f(uniPrefix + "ambientColor", getAmbientColor());

}

void ShadowDirLight::setProjMat(glm::mat4 projMat) {
	this->projMat = projMat;
}

void ShadowDirLight::setViewMat(glm::mat4 viewMat) {
	this->viewMat = viewMat;
}

void ShadowDirLight::setDirection(glm::vec3 direction) {
	this->direction = direction;
	updateViewMat();
}

uint32_t ShadowDirLight::getShadowMapID() const {
	return shadowMap;
}