#pragma once

#ifndef TEXTUREINTERFACE_H
#define TEXTUREINTERFACE_H

#include "logger/logger.h"

typedef unsigned int uint;
typedef unsigned short ushort;

#include <string>

enum imageType{UNKNOWN, PNG, TGA, BMP, JPEG, DDS, EXR, HDR, TIFF};
const ushort IMAGETYPE_COUNT = 9;
const std::string imageType_str[IMAGETYPE_COUNT] = {"UNKNOWN", "PNG", "TGA", "BMP", "JPEG", "DDS", "EXR", "HDR", "TIFF"}

typedef unsigned int uint;

bool caseInsensitive_compare(const string& a, const string& b) {
	if (a.size() != b.size()) return false;
	for (int i = 0; i < a.size(); i++) {
		if (tolower(a[i]) != tolower(b[i])) return false;
	}
	return true;
}

imageType detectExtension(std::string fileName) {
	//uint lastDot=0, lastSlash=0;//  This slash -> /
	
	fileName = fileName.substr(fileName.find_last_of("/\\") + 1);

	std::size_t lastDot;
	lastDot = fileName.find_last_of(".");
	if (lastDot == std::string::npos) {//If dot is not found
		return UNKNOWN;
	}

	fileName = fileName.substr(lastDot+1);
	//TODO: Make mime type table + checker somewhere else
	for (ushort i = 1; i < IMAGETYPE_COUNT; i++) {
		if (caseInsensitive_compare(fileName, imageType_str[i])) return i;
	}
	return 0;
}

class textureInterface{

public:
	vector<float> image;
	uint width=0, height=0;
	std::string colorFormat = "rgb";

	textureInterface() = default;

	textureInterface(const std::string& fileName) { loadFile(fileName); }

	textureInterface(vector<float> image, uint width, uint height, std::string& colorFormat):
		image(image), width(width), height(height), colorFormat(colorFormat) {}

	textureInterface(const textureInterface&) = default;

	template<imageType iType> bool loadFile(const std::string& fileName){//TODO: Think of better name for iType
		LOG("Loading for image type %i(%s) not implemented!", iType, imageType_str[iType].data());
		return false;
	}

	template<imageType iType> bool saveFile(const std::string& fileName) {//TODO: Think of better name for iType
		LOG("Saving for image type %i(%s) not implemented!", iType, imageType_str[iType].data());
		return false;
	}

	~textureInterface() {}

};

template<> bool textureInterface::loadFile<UNKNOWN>(const std::string& fileName) {
	LOG("Attempting to load image of unknown file type! File name: %s", fileName.data());
	return false;
}

template<> bool textureInterface::saveFile<UNKNOWN>(const std::string& fileName) {
	LOG("Attempting to save image of unknown file type! File name: %s", fileName.data());
	return false;
}

#endif
