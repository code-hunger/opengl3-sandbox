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
	uint size = 0;
	std::string format = "rgb";

	TextureInterface() = default;

	TextureInterface(vector<float> image, uint size, std::string& format)
	    : image(image), size(size), format(format)
	{}

	TextureInterface(TextureInterface&& other)
	    : image(other.image), size(other.size), format(other.format)
	{
		other.image.clear();
		other.size = 0;
	}

	~TextureInterface() {}

};

#endif
