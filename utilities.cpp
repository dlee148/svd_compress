#include "utilities.h"

std::string formatTmpImgPath(std::string file) {
	std::string ext = "", prefix = "";
	int slashIndex = 0, dotIndex = 0;
	
	for (int i = 0; i < file.length(); i++) {
		if (file[i] == '/') slashIndex = i;
		if (file[i] == '.') dotIndex = i;
	}
	
	ext = file.substr(dotIndex, file.length() - dotIndex);
	prefix = file.substr(0, slashIndex + 1);
	
	return prefix + std::to_string(time(0)) + ext;
}

std::ifstream::pos_type filesize(const char* filename) {
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

void printCompression(std::string oldFile, std::string newFile) {
	float compressed = 100.0 - (((float)filesize(newFile.c_str()) / (float)filesize(oldFile.c_str())) * 100);
	std::cout << "Compressed by " << compressed << "%" << std::endl;
}

void throwError(int errorCode) {
	switch (errorCode) {
		case USAGE_ERROR:
			std::cout << "Usage: ./svd_compress IMAGE_FILE COMPRESSION_FACTOR (Percentage)" << std::endl;
			break;
		case COMPRESSION_FACTOR_ERROR:
			std::cout << "Compression factor must be an integer between 0 and 100." << std::endl;
			break;
		case FILE_NOT_FOUND_ERROR:
			std::cout << "File does not exist." << std::endl;
			break;
		default:
			std::cout << "An error occurred." << std::endl;
			break;
	}
	
	exit(0);
}
