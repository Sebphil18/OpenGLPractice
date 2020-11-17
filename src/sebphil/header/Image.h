#pragma once

struct Image {
	int width;
	int height;
	int channels;
	unsigned char* buffer;
};