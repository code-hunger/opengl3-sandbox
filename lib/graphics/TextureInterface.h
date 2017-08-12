#pragma once

#ifndef TEXTUREINTERFACE_H
#define TEXTUREINTERFACE_H

#include "logger/logger.h"

#include <string>
#include <vector>

typedef unsigned int uint;

class TextureInterface
{
public:
	vector<float> image;
	uint width = 0, height = 0; //Width = x, height = y
	std::string format = "rgb";

	TextureInterface() = default;

	TextureInterface(vector<float> image, uint width, uint height, std::string& format)
	    : image(image), width(width), height(height), format(format)
	{}

	TextureInterface(TextureInterface&& other)
	    : image(other.image), height(other.height), width(other.width) format(other.format)
	{
		other.image.clear();
		other.height = 0;
		other.width = 0;
	}

	~TextureInterface() {}

};

#endif
