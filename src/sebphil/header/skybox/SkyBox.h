#pragma once
#include "shader/ShaderProgram.h"
#include "globjects/VertexBuffer.h"
#include "globjects/VertexBufferLayout.h"
#include "globjects/VertexArray.h"
#include "image/ImageLoader.h"
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
	void setTexData(ImageLoader& imgLoader, unsigned int texture);
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