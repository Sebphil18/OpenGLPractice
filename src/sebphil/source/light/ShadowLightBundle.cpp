#include <memory>
#include "light/ShadowLightBundle.h"

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

void ShadowLightBundle::update(std::vector<std::shared_ptr<Model>>& models, ShaderProgram& shadowProgram, ShaderProgram& pointShadowProgram, ShaderProgram& program) {
    dirLight.update(models, shadowProgram, program);
    pointLight.update(models, pointShadowProgram, program);
}