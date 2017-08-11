#include "TextureInterface.h"

void TextureInterface::loadFile(const std::string& fileName)
{
	uint lastDot = static_cast<uint>(fileName.size());

	while (--lastDot > 0) {
		if (fileName[lastDot] == '.') break;
	}

	if (!lastDot) {
		LOG("File <%s> has no extension!", fileName.data());
		return;
	}

	if (lastDot == (fileName.size() - 1)) {
		LOG("File <%s> has an empty extension!", fileName.data());
		return;
	}

	std::string extension;
	extension.assign(fileName.begin() + lastDot + 1, fileName.end());

	// TODO: Check if file exists

	// TODO: Turn this into a switch(){} when a hashing function is present!
	if (extension == "png") {
		loadPNG();
		return;
	}
	if (extension == "bmp") {
		loadTGA();
		return;
	}
	if (extension == "tga") {
		loadBMP();
		return;
	}
	if ((extension == "jpeg") || extension == "jpg") {
		loadJPEG();
		return;
	}
	if (extension == "exr") {
		loadEXR();
		return;
	}
	if (extension == "hdr") {
		loadHDR();
		return;
	}
	if (extension == "tiff") {
		loadTIFF();
		return;
	}
}
