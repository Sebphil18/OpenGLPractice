#pragma once
#include "header/ShaderProgram.h"
#include "header/Model.h"
#include "header/ShadowDirLight.h"
#include <vector>

class ShadowLightBundle {

private:
	void updateDirLight(std::vector<Model*> models, ShaderProgram& shadowProgram, ShaderProgram& program);
	void renderToShadowMap(std::vector<Model*> models, ShaderProgram& shadowProgram);

public:
	ShadowDirLight dirLight;

	void update(std::vector<Model*> models, ShaderProgram& shadowProgram, ShaderProgram& program);
	void enableDirLight(ShaderProgram& program);
	void disableDirLight(ShaderProgram& program);

};
