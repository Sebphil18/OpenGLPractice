#pragma once
#include "header/ShaderProgram.h"
#include "header/VertexBuffer.h"
#include "header/VertexBufferLayout.h"
#include "header/VertexArray.h"
#include "header/Image.h"
#include <string>

class SkyBox {

private:

	static float cubeVertices[];

	uint32_t textureID;

	VertexBuffer vbo;
	VertexBufferLayout vboLayout;
	VertexArray vao;

	std::string paths[6];
	
	void initilizeSkyBox();
	void generateTexID();
	void setUpBuffer();

	void bindTexture();
	Image loadImg(const std::string& path);
	void setTexData(Image& img, unsigned int texture);
	void setTexParams();

	void setUpDepthTest();
	void drawVertices();
	void restoreDepthTest();

public:

	SkyBox();
	SkyBox(std::string paths[]);

	void loadTextures(std::string paths[]);
	void draw(ShaderProgram& program);

};