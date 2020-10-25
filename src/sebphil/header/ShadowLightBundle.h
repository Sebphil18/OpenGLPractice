#pragma once
#include "header/ShaderProgram.h"
#include "header/Model.h"
#include "header/ShadowDirLight.h"
#include "header/ShadowPointLight.h"
#include <vector>

class ShadowLightBundle {

private:
	void updateDirLight(std::vector<Model*>& models, ShaderProgram& shadowProgram, ShaderProgram& program);
	void renderToShadowMap(std::vector<Model*>& models, ShaderProgram& shadowProgram);

	void updatePointLight(std::vector<Model*>& models, ShaderProgram& pointShadowProgram, ShaderProgram& program);

public:
	ShadowDirLight dirLight;
	ShadowPointLight pointLight;

	void update(std::vector<Model*>& models, ShaderProgram& shadowProgram, ShaderProgram& pointShadowProgram, ShaderProgram& program);

	void enableDirLight(ShaderProgram& program);
	void disableDirLight(ShaderProgram& program);

	void enablePointLight(ShaderProgram& program);
	void disablePointLight(ShaderProgram& program);

};
