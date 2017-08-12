
#ifndef TEXTURELOADER_H

#define TEXTURELOADER_H

typedef unsigned int uint;
typedef unsigned short ushort;

enum imageType{UNKNOWN, PNG, TGA, BMP, JPEG, DDS, EXR, HDR, TIFF};
const ushort IMAGETYPE_COUNT = 9;
const std::string imageType_str[IMAGETYPE_COUNT] = {"UNKNOWN", "PNG", "TGA", "BMP", "JPEG", "DDS", "EXR", "HDR", "TIFF"}//When we have a mime type table handy, delete these

class TextureLoader{

};

#endif