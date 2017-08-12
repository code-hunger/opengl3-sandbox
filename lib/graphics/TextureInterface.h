#pragma once

#ifndef TEXTUREINTERFACE_H
#define TEXTUREINTERFACE_H

#include "logger/logger.h"

#include <string>

typedef unsigned int uint;

class TextureInterface
{
public:
	float* data = nullptr;
	uint size = 0;
	std::string format = "rgb";

	TextureInterface() = default;

	TextureInterface(float data[], uint size, std::string& format)
	    : data(data), size(size), format(format)
	{}

	TextureInterface(TextureInterface&& other)
	    : data(other.data), size(other.size), format(other.format)
	{
		other.data = nullptr;
		other.size = 0;
	}

	~TextureInterface() { delete[] data; }

};

#endif
