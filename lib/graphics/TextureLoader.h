
#ifndef TEXTURELOADER_H

#define TEXTURELOADER_H

#include "logger/logger.h"
#include <string>

typedef unsigned int uint;
typedef unsigned short ushort;

enum imageType{UNKNOWN, PNG, TGA, BMP, JPEG, DDS, EXR, HDR, TIFF};
const ushort IMAGETYPE_COUNT = 9;
const std::string imageType_str[IMAGETYPE_COUNT] = {"UNKNOWN", "PNG", "TGA", "BMP", "JPEG", "DDS", "EXR", "HDR", "TIFF"}//When we have a mime type table handy, delete these


bool caseInsensitive_compare(const string& a, const string& b) {
	if (a.size() != b.size()) return false;
	for (int i = 0; i < a.size(); i++) {
		if (tolower(a[i]) != tolower(b[i])) return false;
	}
	return true;
}

imageType findExtension(std::string fileName) {
	//uint lastDot=0, lastSlash=0;//  This slash -> /
	
	fileName = fileName.substr(fileName.find_last_of("/\\") + 1);//<- this works for both slashes, apparently

	std::size_t lastDot;
	lastDot = fileName.find_last_of(".");
	if (lastDot == std::string::npos) {//If dot is not found
		return UNKNOWN;
	}

	fileName = fileName.substr(lastDot+1);
	//TODO: Mime type reminder
	for (ushort i = 1; i < IMAGETYPE_COUNT; i++) {
		if (caseInsensitive_compare(fileName, imageType_str[i])) return i;
	}
	return UNKNOWN;
}

class TextureLoader{

	//Both of these should return whether they were successful or not
	template<imageType iType> bool loadFile(const std::string& fileName){//TODO: Think of better name for iType
		LOG("Loading for image type %i(%s) not implemented!", iType, imageType_str[iType].data());
		return false;
	}

	template<imageType iType> bool saveFile(const std::string& fileName) {//TODO: Think of better name for iType
		LOG("Saving for image type %i(%s) not implemented!", iType, imageType_str[iType].data());
		return false;
	}

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