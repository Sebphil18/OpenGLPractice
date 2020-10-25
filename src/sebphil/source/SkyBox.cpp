#include "header/SkyBox.h"
#include "glad/glad.h"
#include "stbi/stb_Image.h"
#include <iostream>

SkyBox::SkyBox() {
    initilizeSkyBox();
}

SkyBox::SkyBox(std::string path[]) {
    initilizeSkyBox();
    loadTextures(path);
}

void SkyBox::initilizeSkyBox() {

    generateTexID();
    vboLayout.addElement({ 3, GL_FLOAT, GL_FALSE });
    setUpBuffer();
}

void SkyBox::generateTexID() {
    glGenTextures(1, &textureID);
}

void SkyBox::setUpBuffer() {
    vbo.bind();
    vao.bind();

    vbo.setData(108 * sizeof(float), &cubeVertices[0], GL_STATIC_DRAW);
    vboLayout.bindLayout();

    vbo.unbind();
    vao.unbind();
}

// TODO: too long
void SkyBox::loadTextures(std::string paths[]) {

    bindTexture();

    for (size_t i = 0; i < 6; i++) {

        const std::string& path = paths[i];
        
        ImageLoader img;
        img.loadRGB(path);
        setTexData(img, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

    }

    setTexParams();

}

Image SkyBox::loadImg(const std::string& path) {
    Image img;
    img.buffer = stbi_load(path.c_str(), &img.width, &img.height, &img.channels, 3);
    return img;
}

void SkyBox::setTexData(ImageLoader& imgLoader, unsigned int texture) {

    glm::vec2 dimensions = imgLoader.getDimensions();
    glTexImage2D(texture, 0, GL_RGB, dimensions.x, dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, imgLoader.getBuffer());

}

void SkyBox::setTexParams() {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void SkyBox::draw(ShaderProgram& program) {
    program.use();
    
    setUpDepthTest();
    bindTexture();

    vao.bind();
    drawVertices();
    vao.unbind();

    restoreDepthTest();
}

void SkyBox::setUpDepthTest() {
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
}

void SkyBox::bindTexture() {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void SkyBox::drawVertices() {
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void SkyBox::restoreDepthTest() {
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}

float SkyBox::cubeVertices[] = {

    // back
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    
    // left
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    // right
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    // front
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    // top
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    // bottom
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};