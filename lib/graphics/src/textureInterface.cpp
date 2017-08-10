#include "textureInterface.h"
#include "logger\logger.h";

#include<string>

typedef unsigned int uint;

class textureInterface{

public:
	float *data;
	uint size;
	std::string fmt, fileName;

	textureInterface() {
		size = 0;
		fmt = std::string("rgb");
		fileName = std::string("empty");
	}

	textureInterface(std::string &_fileName) {
		loadFile(_fileName);
	}

	textureInterface(float _data[], uint _size, std::string &_fmt, std::string &_fileName) {//TODO: format and filename not required
		copyData(_data, _size, _fmt, _fileName);
	}

	textureInterface(textureInterface &_texIntf) {
		data = _texIntf.data;
		size = _texIntf.size;
		fmt = _texIntf.fmt;
		fileName = _texIntf.fileName;
	}

	//Load and copy methods return whether they were successful or not.
	bool loadFile(std::string &_fileName) {
		uint i = 0, lastDot = 0;
		fileName = _fileName;

		for (i = 0; i<fileName.size(); i++) {
			if (fileName[i] == '.') {
				lastDot = i;
			}
		}

		if (!lastDot) { 
			LOG("File <%s> has no extension!", _fileName.data());
			return;
		}

		if (lastDot == (_fileName.size() - 1)) {
			LOG("File <%s> has an empty extension!", _fileName.data());
			return;
		}

		std::string extension;
		extension.assign(_fileName.begin()+lastDot+1,_fileName.end());

		//TODO: Check if file exists

		//TODO: Turn this into a switch(){} when a hashing function is present!

		if (extension == std::string("png")) { loadPNG(); return; }
		if (extension == std::string("bmp")) { loadTGA(); return; }
		if (extension == std::string("tga")) { loadBMP(); return; }
		if ( (extension == std::string("jpeg")) || extension == std::string("jpg"))  loadJPEG();

		if (extension == std::string("exr"))  { loadEXR(); return;  }
		if (extension == std::string("hdr"))  { loadHDR(); return;  }
		if (extension == std::string("tiff")) { loadTIFF(); return; }


		
	}

	bool copyData(float _data[], uint _size, std::string _fmt, std::string _fileName) {//TODO: format and filename not required!
		data = _data;
		size = _size;
		fmt = _fmt;
		fileName = _fileName;
	}

	~textureInterface() {
		delete[] data;
	}




private:

	//All load methods return whether they were successful or not. 
	bool loadPNG() {
		LOG("PNG loading not implemented yet!");
		return false;
	}
	bool savePNG() {
		LOG("PNG saving not implemented yet!");
		return false;
	}


	bool loadTGA() {
		LOG("Targa loading not implemented yet!");
		return false;
	}
	bool saveTGA() {
		LOG("Targa saving not implemented yet!");
		return false;
	}


	bool loadBMP() {
		LOG("Bitmap loading not implemented yet!");
		return false;
	}
	bool saveBMP() {
		LOG("Bitmap saving not implemented yet!");
		return false;
	}


	bool loadJPEG() {
		LOG("JPEG loading not implemented yet!");
		//Do I look like I know what a jpeg is?
		//I just want a picture of a got dang hot dawg!
		//https://www.youtube.com/watch?v=ZXVhOPiM4mk
		return false;
	}
	bool saveJPEG() {
		LOG("JPEG saving not implemented yet!");
		return false;
	}
	




	bool loadEXR() {
		LOG("EXR loading not implemented yet!");
		return false;
	}
	bool saveEXR() {
		LOG("EXR loading not implemented yet!");
		return false;
	}

	bool loadHDR() {
		LOG("High dynamic range format loading not implemented yet!");
		return false;
	}
	bool saveHDR() {
		LOG("High dynamic range format loading not implemented yet!");
		return false;
	}

	bool loadTIFF() {
		LOG("TIFF loading not implemented yet!");
		return false;
	}
	bool saveTIFF() {
		LOG("TIFF loading not implemented yet!");
		return false;
	}
};


