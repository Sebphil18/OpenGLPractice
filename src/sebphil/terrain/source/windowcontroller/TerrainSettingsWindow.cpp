#include "windowcontroller/TerrainSettingsWindow.h"
#include "imgui/imgui.h"

TerrainSettingsWindow::TerrainSettingsWindow(const std::shared_ptr<TerrainModel>& terrain) :
	terrain(terrain), noiseGenerator(terrain->getNoiseGeneratorPtr()), heightMap(terrain->getHeightMapPtr()) {
    seed = noiseGenerator->getSeed();
    lengthX = heightMap->getLengthX();
    lengthY = heightMap->getLengthY();
};

void TerrainSettingsWindow::draw() {
    ImGui::Begin("terrain-settings");

    drawNoiseSettings();
    ImGui::Separator();

    drawHeightMapSettings();
    ImGui::Separator();

    drawScaleSlider();

    ImGui::End();
}

void TerrainSettingsWindow::drawNoiseSettings() {
    int* octaves = noiseGenerator->getOctavesPtr();
    float* frq = noiseGenerator->getFrequencyPtr();
    float* ampl = noiseGenerator->getAmplitudePtr();
    float* frqMultiplier = noiseGenerator->getFreqMultiplierPtr();
    float* amplMultiplier = noiseGenerator->getAmplMultiplierPtr();

    addUpdateSliderFloat("frequency", frq, 0, 0.4);
    addUpdateSliderFloat("amplitude", ampl, 0, 40);
    addUpdateSliderFloat("frequency-multiplier", frqMultiplier, 0.1, 2);
    addUpdateSliderFloat("amplitude-multiplier", amplMultiplier, 0.1, 2);
    addUpdateSliderInt("octaves", octaves, 1, 10);
}

void TerrainSettingsWindow::addUpdateSliderFloat(const char* title, float* value, float min, float max) {
    ImGui::PushItemWidth(SEB_GUI_SLIDER_WIDTH);
    if (ImGui::SliderFloat(title, value, min, max))
        terrain->update();
}

void TerrainSettingsWindow::addUpdateSliderInt(const char* title, int* value, int min, int max) {
    ImGui::PushItemWidth(SEB_GUI_SLIDER_WIDTH);
    if (ImGui::SliderInt(title, value, min, max))
        terrain->update();
}

void TerrainSettingsWindow::drawHeightMapSettings() {
    addResizeSliders(&lengthX, &lengthY, 20, 1000);
    addSeedInput(&seed);
}

void TerrainSettingsWindow::addResizeSliders(int* lengthX, int* lengthY, int min, int max) {
    ImGui::PushItemWidth(SEB_GUI_SLIDER_WIDTH);
    if (ImGui::SliderInt("lengthX", lengthX, min, max))
        terrain->resize(*lengthX, *lengthY);

    ImGui::PushItemWidth(SEB_GUI_SLIDER_WIDTH);
    if (ImGui::SliderInt("lengthY", lengthY, min, max))
        terrain->resize(*lengthX, *lengthY);
}

void TerrainSettingsWindow::addSeedInput(int* value) {
    ImGui::PushItemWidth(SEB_GUI_SLIDER_WIDTH);
    if (ImGui::InputInt("seed", value)) {
        noiseGenerator->setSeed(*value);
        terrain->update();
    }
}

void TerrainSettingsWindow::drawScaleSlider() {
    if (ImGui::SliderFloat("scale", terrain->getScalePtr(), 0.1, 1)) {
        terrain->update(false);
    }
}