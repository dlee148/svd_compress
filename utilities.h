#ifndef utilities_h
#define utilities_h

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include "constants.h"

std::string formatTmpImgPath(std::string file);
std::ifstream::pos_type filesize(const char* filename);
void printCompression(std::string oldFile, std::string newFile);
void throwError(int errorCode);

#endif
