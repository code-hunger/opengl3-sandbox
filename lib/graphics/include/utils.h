#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <string>

std::string readFile(std::ifstream input)
{
	std::string s;
	getline(input, s, '\0');
	return s;
}

std::string readFile(std::string fileName)
{
	return readFile(std::ifstream(fileName));
}

inline std::string readFile(const char* path, const char* fileName)
{
	return readFile((std::string(path) + '/' + fileName).c_str());
}

#endif
