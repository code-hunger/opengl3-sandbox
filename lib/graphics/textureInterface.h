#pragma once

#ifndef TEXTUREINTERFACE_H
#define TEXTUREINTERFACE_H

#include <string>

typedef unsigned int uint;

class textureInterface{
public:

	textureInterface();
	textureInterface(std::string &_fileName);
	textureInterface(float _data[], uint _size, std::string _fmt);
	textureInterface(textureInterface &_texIntf);

	//Load and copy methods return whether they were successful or not.
	bool loadFile(std::string &_fileName);
	bool copyData(float _data[], uint _size, std::string _fmt);

	~textureInterface();
private:

	//All load methods return whether they were successful or not. 
	bool loadPNG(); bool savePNG();
	bool loadTGA(); bool saveTGA();
	bool loadBMP(); bool saveBMP();
	bool loadJPEG(); bool saveJPEG();//Did you know? Early windows supported 3-letter extensions only, so no jpeg for them; but macintosh supported more, so the original and more correct term would be jpeg.
	// ^Non-float image formats
	//
	// \/ Float image formats
	bool loadEXR(); bool saveEXR();
	bool loadHDR(); bool saveHDR();
	bool loadTIFF(); bool saveTIFF();
};

#endif