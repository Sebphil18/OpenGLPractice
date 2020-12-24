#pragma once
#include <memory>
#include "terrainmodel/TerrainModel.h"
#include "heightmap/SpecialEffect.h"
#include "erosion/HydraulicErosion.h"

#ifndef SEB_GUI_SLIDER_WIDTH
#define SEB_GUI_SLIDER_WIDTH 200
#endif

class HydraulicErosionWindow {

private:
	std::shared_ptr<TerrainModel> terrain;
	HeightMap& heightmap;
	HydraulicErosion erosion;
	DropletSettings* settings;
	SpecialEffect blurEffect;
	SpecialEffect sharpenEffect;

	void drawSettingsSlider();
	void drawInputInt(const std::string& title, int* value, int incrementSlow, int incrementFast);
	void drawSliderInt(const std::string& title, int* value, int min, int max);
	void drawSliderFloat(const std::string& title, float* value, float min, float max);
	void drawEffectButton(const std::string& title, const SpecialEffect& effect);
	void drawErodButton();
	void drawResetButton();

public:
	HydraulicErosionWindow(const std::shared_ptr<TerrainModel>& terrain);

	void draw();
};
