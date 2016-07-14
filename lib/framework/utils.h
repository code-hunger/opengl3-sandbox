#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <string>

std::string readFile(const char* fileName)
{
	printf("Reading file %s\n", fileName);
	std::string s;
	getline(std::ifstream(fileName), s, '\0');
	return s;
}

#endif
