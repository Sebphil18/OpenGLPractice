#include "windowcontroller/ShaderSettingsWindow.h"
#include "imgui/imgui.h"

ShaderSettingsWindow::ShaderSettingsWindow(ShaderProgram* const program) :
    program(program), parallaxStrength(0.08), minDepthLayers(20), maxDepthLayers(40),
    minShadowBias(0.005),
    layerLevel(0), layerSlope(1), layerBlend(0),
    textureScaling(0.5) 
{}

void ShaderSettingsWindow::draw() {

    if (program == nullptr)
        return;

    ImGui::Begin("shader-settings");

    drawParallaxMappingSettings();
    drawShadowMappingSettings();
    drawTextureShadingSettings();

    ImGui::End();

}

void ShaderSettingsWindow::drawParallaxMappingSettings() {

    if (ImGui::CollapsingHeader("parallax-mapping", false)) {

        if (ImGui::SliderFloat("parallax-strength", &parallaxStrength, 0, 1)) {
            program->setUniform1f("parallaxStrength", parallaxStrength);
        }

        if (ImGui::SliderInt("min depth-layers", &minDepthLayers, 0, 80)) {
            program->setUniform1i("minDepthLayers", minDepthLayers);
        }

        if (ImGui::SliderInt("max depth-layers", &maxDepthLayers, 0, 100)) {
            program->setUniform1i("maxDepthLayers", maxDepthLayers);
        }

    }

}

void ShaderSettingsWindow::drawShadowMappingSettings() {

    if (ImGui::CollapsingHeader("shadow-mapping", false)) {

        if (ImGui::SliderFloat("minimal shadow-bias", &minShadowBias, 0, 0.1)) {
            program->setUniform1f("minShadowBias", minShadowBias);
        }

    }

}

void ShaderSettingsWindow::drawTextureShadingSettings() {

    if (ImGui::CollapsingHeader("texture-shading", false)) {

        drawLayerSettings();

        if (ImGui::SliderFloat("texture-scaling", &textureScaling, 0, 1)) {
            program->setUniform1f("textureScaling", textureScaling);
        }

    }

}

void ShaderSettingsWindow::drawLayerSettings() {

    if (ImGui::SliderFloat("layer-level", &layerLevel, -30, 50)) {
        program->setUniform1f("layers[0].maxLevel", layerLevel);
    }

    if (ImGui::SliderFloat("layer-slope", &layerSlope, 0.01, 50)) {
        program->setUniform1f("layers[0].slope", layerSlope);
    }

    if (ImGui::SliderFloat("layer-blend", &layerBlend, 0, 1)) {
        program->setUniform1f("layers[0].blend", layerBlend);
    }

}