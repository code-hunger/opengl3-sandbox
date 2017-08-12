
#ifndef TEXTURELOADER_H

#define TEXTURELOADER_H

#include "logger/logger.h"
#include <string>

typedef unsigned int uint;
typedef unsigned short ushort;

enum imageType{UNKNOWN, PNG, TGA, BMP, JPEG, DDS, EXR, HDR, TIFF};
const ushort IMAGETYPE_COUNT = 9;
const std::string imageType_str[IMAGETYPE_COUNT] = {"UNKNOWN", "PNG", "TGA", "BMP", "JPEG", "DDS", "EXR", "HDR", "TIFF"}//When we have a mime type table handy, delete these

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

#endif