#pragma once
#include "shader/ShaderProgram.h"

class ShaderSettingsWindow {

private:
	ShaderProgram* const program;

	int minDepthLayers, maxDepthLayers;
	float parallaxStrength;

	float minShadowBias;

	float layerLevel, layerSlope, layerBlend, textureScaling;

public:
	ShaderSettingsWindow(ShaderProgram* const shaderProgram);

	void draw();

	void drawParallaxMappingSettings();
	void drawShadowMappingSettings();
	void drawTextureShadingSettings();
	
	void drawLayerSettings();

};