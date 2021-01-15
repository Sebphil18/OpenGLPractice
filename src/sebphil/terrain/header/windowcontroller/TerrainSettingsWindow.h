#pragma once
#include <memory>
#include "terrainmodel/TerrainModel.h"

#ifndef SEB_GUI_SLIDER_WIDTH
#define SEB_GUI_SLIDER_WIDTH 200
#endif

class TerrainSettingsWindow {
private:
	std::shared_ptr<TerrainModel> terrain;
	FractalNoiseGenerator* noiseGenerator;
	HeightMap* heightMap;

	int seed;
	int lengthX, lengthY;

	void drawNoiseSettings();
	void drawHeightMapSettings();

	void addUpdateSliderFloat(const char* title, float* value, float min, float max);
	void addUpdateSliderInt(const char* title, int* value, int min, int max);
	void addResizeSliders(int* lengthX, int* lengthY, int min, int max);
	void addSeedInput(int* value);

	void drawScaleSlider();

public:
	TerrainSettingsWindow(const std::shared_ptr<TerrainModel>& terrain);

	void draw();

};
