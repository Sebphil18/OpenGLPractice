#include "header/ShadowPointLight.h"
#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"

ShadowPointLight::ShadowPointLight():
	shadowWidth(1024), shadowHeight(1024), near(0.1), far(500) {

	setUpTexture();
	setUpFramebuffer();

	aspRatio = shadowWidth / shadowHeight;

	generateProjMat();
	generateLightSpaceMat();

}

void ShadowPointLight::setUpTexture() {
	generateTexture();
	bindTexture();
	fillTexture();
	setTexParams();
}

void ShadowPointLight::generateTexture() {
	glGenTextures(1, &cubeMap);
}

void ShadowPointLight::bindTexture() {
	glActiveTexture(GL_TEXTURE26);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
}

void ShadowPointLight::fillTexture() {
	for (uint32_t i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
}

void ShadowPointLight::setTexParams() {
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void ShadowPointLight::setUpFramebuffer() {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeMap, 0);
	glReadBuffer(GL_NONE);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowPointLight::generateProjMat() {
	projMat = glm::perspective(glm::radians(90.0f), aspRatio, near, far);
}

void ShadowPointLight::generateLightSpaceMat() {
	lightSpaceMat[0] = projMat * glm::lookAt(position, position + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0));
	lightSpaceMat[1] = projMat * glm::lookAt(position, position + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0));
	lightSpaceMat[2] = projMat * glm::lookAt(position, position + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
	lightSpaceMat[3] = projMat * glm::lookAt(position, position + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
	lightSpaceMat[4] = projMat * glm::lookAt(position, position + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0));
	lightSpaceMat[5] = projMat * glm::lookAt(position, position + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
}

void ShadowPointLight::update(const std::vector<Model*>& models, ShaderProgram& shadowProgram, ShaderProgram& program) {

	std::string uniName = "shadowPointLights[0].";

	shadowProgram.setUniform1f("far", far);
	shadowProgram.setUniformVec3f("lightPos", position);

	updateShadow(program, shadowProgram);
	updateLightSpaceMat(shadowProgram);
	renderModels(models, shadowProgram);

	updateK(program, uniName);
	updatePosition(program, uniName);
	updateColors(program, uniName);

}

void ShadowPointLight::updateLightSpaceMat(ShaderProgram& shadowProgram) {
	shadowProgram.setUniformMat4f("lightSpaceMat[0]", lightSpaceMat[0]);
	shadowProgram.setUniformMat4f("lightSpaceMat[1]", lightSpaceMat[1]);
	shadowProgram.setUniformMat4f("lightSpaceMat[2]", lightSpaceMat[2]);
	shadowProgram.setUniformMat4f("lightSpaceMat[3]", lightSpaceMat[3]);
	shadowProgram.setUniformMat4f("lightSpaceMat[4]", lightSpaceMat[4]);
	shadowProgram.setUniformMat4f("lightSpaceMat[5]", lightSpaceMat[5]);
}

void ShadowPointLight::renderModels(const std::vector<Model*>& models, ShaderProgram& shadowProgram) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, shadowWidth, shadowHeight);

	for (std::size_t i = 0; i < models.size(); i++) {
		shadowProgram.setUniformMat4f("worldMatrix", models[i]->getWorldMat());
		models[i]->draw(shadowProgram);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowPointLight::updateShadow(ShaderProgram& program, ShaderProgram& shadowProgram) {
	shadowProgram.setUniform1f("far", far);
	shadowProgram.setUniformVec3f("lightPos", position);

	program.setUniform1i("shadowPointLightsCount", 1);
	program.setUniform1i("pointShadowMap", 26);
	program.setUniform1f("far", far);
}

void ShadowPointLight::setPosition(glm::vec3 position) {
	this->position = position;
	generateLightSpaceMat();
}