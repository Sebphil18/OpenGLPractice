#include "header/ImageLoader.h"
#include "stbi/stb_Image.h"
#include <iostream>

ImageLoader::ImageLoader() {
	img.width = 0;
	img.height = 0;
	img.channels = 3;
	img.buffer = nullptr;
}
ImageLoader::~ImageLoader() {
	stbi_image_free(img.buffer);
}

void ImageLoader::loadRGB(const std::string& imgPath) {
	img.buffer = stbi_load(imgPath.c_str(), &img.width, &img.height, &img.channels, 3);

	if (!img.buffer)
		std::cout << "ERROR::IMAGELOADER::LOADRGB::Could not load: " << imgPath << std::endl;
}

void ImageLoader::loadRGBA(const std::string& imgPath) {
	img.buffer = stbi_load(imgPath.c_str(), &img.width, &img.height, &img.channels, 4);

	if (!img.buffer)
		std::cout << "ERROR::IMAGELOADER::LOADRGB::Could not load: " << imgPath << std::endl;
}

glm::vec2 ImageLoader::getDimensions() const {
	return glm::vec2(img.width, img.height);
}

unsigned char* ImageLoader::getBuffer() const {
	return img.buffer;
}