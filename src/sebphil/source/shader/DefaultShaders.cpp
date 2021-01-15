#include "shader/DefaultShaders.h"

DefaultShaders::DefaultShaders():
	standardProgram("src/sebphil/shader/vertex/VertexPhong.glsl", "src/sebphil/shader/geometry/GeoPhong.glsl", "src/sebphil/shader/fragment/FragmentPhong.glsl"),
	skyboxProgram("src/sebphil/shader/vertex/VertexSkyBox.glsl", "src/sebphil/shader/fragment/FragmentSkyBox.glsl"),
	dirShadowProgram("src/sebphil/shader/vertex/VertexShadow.glsl", "src/sebphil/shader/fragment/FragmentShadow.glsl"),
	pointShadowProgram("src/sebphil/shader/vertex/VertexPointShadow.glsl", "src/sebphil/shader/geometry/GeoPointShadow.glsl", "src/sebphil/shader/fragment/FragmentPointShadow.glsl")
{
	skyboxProgram.setUniform1i("skybox", 1);
}