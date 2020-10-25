#include "header/ShadowLightBundle.h"

void ShadowLightBundle::enableDirLight(ShaderProgram& program) {
    program.setUniform1i("shadowDirLightsCount", 1);
    program.setUniform1i("shadowMap", 20);

    glActiveTexture(GL_TEXTURE20);
    glBindTexture(GL_TEXTURE_2D, dirLight.getShadowMapID());
}

void ShadowLightBundle::disableDirLight(ShaderProgram& program) {
    program.setUniform1i("shadowDirLightsCount", 0);
}

void ShadowLightBundle::enablePointLight(ShaderProgram& program) {
    program.setUniform1i("shadowPointLightsCount", 1);
}

void ShadowLightBundle::disablePointLight(ShaderProgram& program) {
    program.setUniform1i("shadowPointLightsCount", 0);
}

void ShadowLightBundle::update(std::vector<Model*>& models, ShaderProgram& shadowProgram, ShaderProgram& pointShadowProgram, ShaderProgram& program) {
    updateDirLight(models, shadowProgram, program);
    updatePointLight(models, pointShadowProgram, program);
}

void ShadowLightBundle::updateDirLight(std::vector<Model*>& models, ShaderProgram& shadowProgram, ShaderProgram& program) {

    dirLight.update(program);
    dirLight.bindFbo();
    dirLight.adjustViewport();

    shadowProgram.setUniformMat4f("dirLightSpaceMat", dirLight.getLightSpaceMat());
    program.setUniformMat4f("dirLightSpaceMat", dirLight.getLightSpaceMat());

    renderToShadowMap(models, shadowProgram);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowLightBundle::renderToShadowMap(std::vector<Model*>& models, ShaderProgram& shadowProgram) {
    for (std::size_t i = 0; i < models.size(); i++) {
        shadowProgram.setUniformMat4f("worldMatrix", models[i]->getWorldMat());
        models[i]->draw(shadowProgram);
    }
}

void ShadowLightBundle::updatePointLight(std::vector<Model*>& models, ShaderProgram& pointShadowProgram, ShaderProgram& program) {
    pointLight.update(models, pointShadowProgram, program);
}