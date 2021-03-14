#include "windowcontroller/HydraulicErosionWindow.h"
#include "imgui/imgui.h"
#include <iostream>

// note: This only works because this class also holds the specific shared_ptr, which holds the referenced HeightMap!
HydraulicErosionWindow::HydraulicErosionWindow(const std::shared_ptr<TerrainModel>& terrain) :
	terrain(terrain), erosion(10000), blurEffect(EffectType::blur), sharpenEffect(EffectType::sharpen), heightmap(*terrain->getHeightMapPtr())
	, blurOnErosion(true), iterations(1) {

	this->settings = erosion.getDropletSettingsPtr();
}

void HydraulicErosionWindow::draw() {

	ImGui::Begin("hydraulic erosion (alpha)");
	drawSettingsSlider();

	drawEffectButton("blur", blurEffect);
	ImGui::SameLine();
	drawEffectButton("sharpen", sharpenEffect);
	ImGui::SameLine();
	drawErodButton();
	ImGui::SameLine();
	drawResetButton();
	ImGui::Checkbox("blur on erosion", &blurOnErosion);
	ImGui::End();
}

void HydraulicErosionWindow::drawSettingsSlider() {
	drawInputInt("iterations", &iterations, 1, 10);
	drawInputInt("amount of drops", erosion.getDropAmountPtr(), 1000, 10000);
	drawSliderInt("drop lifetime", &settings->lifetime, 10, 100);
	drawSliderFloat("gravity", &settings->gravity, 0.01, 12);
	drawSliderFloat("erosion-rate", &settings->erosionRate, 0.05, 1);
	drawSliderFloat("deposit-rate", &settings->depositRate, 0.05, 0.5);
	drawSliderFloat("intertia multiplier", &settings->inertiaMultiplier, 0.1, 1);
	drawSliderFloat("capacity multiplier", &settings->capacityMultiplier, 0.1, 32);
	drawSliderFloat("minimal slope", &settings->minSlope, 0.005, 0.5);
	drawSliderFloat("brush radius", &settings->brushRadius, 1, 10);
}

void HydraulicErosionWindow::drawInputInt(const std::string& title, int* value, int incrementSlow, int incrementFast) {
	ImGui::PushItemWidth(SEB_GUI_SLIDER_WIDTH);
	ImGui::InputInt(title.c_str(), value, incrementSlow, incrementFast);
}

void HydraulicErosionWindow::drawSliderInt(const std::string& title, int* value, int min, int max) {
	ImGui::PushItemWidth(SEB_GUI_SLIDER_WIDTH);
	ImGui::SliderInt(title.c_str(), value, min, max);
}

void HydraulicErosionWindow::drawSliderFloat(const std::string& title, float* value, float min, float max) {
	ImGui::PushItemWidth(SEB_GUI_SLIDER_WIDTH);
	ImGui::SliderFloat(title.c_str(), value, min, max);
}

void HydraulicErosionWindow::drawEffectButton(const std::string& title, const SpecialEffect& effect) {
	if (ImGui::Button(title.c_str())) {
		effect.apply(heightmap);
		terrain->update(false);
	}
}

void HydraulicErosionWindow::drawErodButton() {
	if (ImGui::Button("erode")) {
		erodeTerrain();
	}
}

void HydraulicErosionWindow::erodeTerrain() {
	// i is an int because iterations is an int: when iterations is negative and i would be an uint the loop would go for a very long time
	for (int i = 0; i < iterations; i++) {
		erosion.erode(heightmap);
		if (blurOnErosion)
			blurEffect.apply(heightmap);
		terrain->update(false);
	}
}

void HydraulicErosionWindow::drawResetButton() {
	if (ImGui::Button("reset")) {
		terrain->update();
	}
}