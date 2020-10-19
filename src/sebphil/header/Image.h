#pragma once

// TODO: convert to class!
/*
	-> void load(const std::string& img)
	-> void free();

	- Model (ModelLoader?)
	- SkyBoy
*/
struct Image {
	int width;
	int height;
	int channels;
	unsigned char* buffer;
};