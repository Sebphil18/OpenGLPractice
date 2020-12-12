#pragma once
#include <memory>
#include "skybox/SkyBox.h"
#include "modelstructure/ModelLoader.h"
#include "light/LightBundle.h"
#include "light/ShadowLightBundle.h"

struct Scene {
	SkyBox skyBox;
	LightBundle lights;
	ShadowLightBundle shadowLights;
	std::vector<std::shared_ptr<Model>> models;
};