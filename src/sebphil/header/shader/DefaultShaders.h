#pragma once
#include "ShaderProgram.h"

class DefaultShaders {
public:
	ShaderProgram phongProgram;
	ShaderProgram skyboxProgram;
	ShaderProgram dirShadowProgram;
	ShaderProgram pointShadowProgram;
	DefaultShaders();

};