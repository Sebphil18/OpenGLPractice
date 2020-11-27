#pragma once
#include "Image.h"
#include "glm/glm.hpp"
#include <string>

class ImageLoader {

private:

	Image img;

public:

	ImageLoader();
	~ImageLoader();

	void loadRGB(const std::string& imgPath);
	void loadRGBA(const std::string& imgPath);

	glm::vec2 getDimensions() const;
	unsigned char* getBuffer() const;

};