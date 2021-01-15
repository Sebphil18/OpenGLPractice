#pragma once
#include "ShaderProgram.h"

class DefaultShaders {
public:
	ShaderProgram standardProgram;
	ShaderProgram skyboxProgram;
	ShaderProgram dirShadowProgram;
	ShaderProgram pointShadowProgram;
	DefaultShaders();

};