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

	TextureInterface(const std::string& fileName) { loadFile(fileName); }

	TextureInterface(float data[], uint size, std::string& format)
	    : data(data), size(size), format(format)
	{
	}

	TextureInterface(TextureInterface&& other)
	    : data(other.data), size(other.size), format(other.format)
	{
		other.data = nullptr;
		other.size = 0;
	}

	// Load and copy methods return whether they were successful or not.
	// @TODO fix return type
	void loadFile(const std::string& fileName);

	~TextureInterface() { delete[] data; }

private:
	// All load methods return whether they were successful or not.
	bool loadPNG()
	{
		LOG("PNG loading not implemented yet!");
		return false;
	}
	bool savePNG()
	{
		LOG("PNG saving not implemented yet!");
		return false;
	}

	bool loadTGA()
	{
		LOG("Targa loading not implemented yet!");
		return false;
	}
	bool saveTGA()
	{
		LOG("Targa saving not implemented yet!");
		return false;
	}

	bool loadBMP()
	{
		LOG("Bitmap loading not implemented yet!");
		return false;
	}
	bool saveBMP()
	{
		LOG("Bitmap saving not implemented yet!");
		return false;
	}

	bool loadJPEG()
	{
		LOG("JPEG loading not implemented yet!");
		// Do I look like I know what a jpeg is?
		// I just want a picture of a got dang hot dawg!
		// https://www.youtube.com/watch?v=ZXVhOPiM4mk
		return false;
	}
	bool saveJPEG()
	{
		LOG("JPEG saving not implemented yet!");
		return false;
	}

	bool loadEXR()
	{
		LOG("EXR loading not implemented yet!");
		return false;
	}
	bool saveEXR()
	{
		LOG("EXR loading not implemented yet!");
		return false;
	}

	bool loadHDR()
	{
		LOG("High dynamic range format loading not implemented yet!");
		return false;
	}
	bool saveHDR()
	{
		LOG("High dynamic range format loading not implemented yet!");
		return false;
	}

	bool loadTIFF()
	{
		LOG("TIFF loading not implemented yet!");
		return false;
	}
	bool saveTIFF()
	{
		LOG("TIFF loading not implemented yet!");
		return false;
	}
};

#endif
