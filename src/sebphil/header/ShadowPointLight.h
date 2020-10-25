#include "header/PointLight.h"
#include "header/ShaderProgram.h"
#include "header/Model.h"

class ShadowPointLight : public PointLight{

private:

	uint32_t cubeMap;
	uint32_t framebuffer;
	uint32_t shadowWidth;
	uint32_t shadowHeight;

	float far, near, aspRatio;

	glm::mat4 projMat;
	glm::mat4 lightSpaceMat[6];

	void setUpTexture();
	void generateTexture();
	void bindTexture();
	void fillTexture();
	void setTexParams();
	void setUpFramebuffer();
	void generateProjMat();
	void generateLightSpaceMat();

	void updateLightSpaceMat(ShaderProgram& shadowProgram);
	void renderModels(const std::vector<Model*>& models, ShaderProgram& shadowProgram);
	void updateShadow(ShaderProgram& program, ShaderProgram& shadowProgram);

public:
	
	ShadowPointLight();

	void update(const std::vector<Model*>& models, ShaderProgram& shadowProgram, ShaderProgram& program);

	void setPosition(glm::vec3 position);

};