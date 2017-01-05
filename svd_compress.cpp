#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include <fstream>
#include "CImg.h"
using namespace cimg_library;

#define BLACK_AND_WHITE 1
#define COLOR 3

std::string formatTmpImgPath(std::string file);
std::ifstream::pos_type filesize(const char* filename);
void printCompression(std::string oldFile, std::string newFile);

int main(int argc, char** argv) {
	cimg_library::cimg::exception_mode(0);

	if (argc != 3) {
		std::cout << "Usage: ./svd_compress IMAGE_FILE COMPRESSION_FACTOR (Percentage)" << std::endl;
		return 0;
	}

	std::string imgFile = argv[1];
	std::string newImgFile = formatTmpImgPath(argv[1]).c_str();
	int compressionFactor = atoi(argv[2]);

	if (compressionFactor > 99 || compressionFactor < 0) {
		std::cout << "Compression factor must be between 0-99." << std::endl;
		return 0;
	}

	CImg<unsigned char> originalImage;

	try {
		originalImage.load(imgFile.c_str());
	}
	catch (CImgException& e) {
		std::cout << "File does not exist." << std::endl;
		return 0;
	}
	
	compressionFactor = floor((((double)compressionFactor / (double)100) * originalImage._height));

	CImg<> U, S_COL, V, res;
	originalImage.SVD(U, S_COL, V);
	CImg<> S(V._width, U._width, 1, originalImage._spectrum == BLACK_AND_WHITE ? 1 : 3);

	for (int i = 0; i < S_COL._height - compressionFactor; i++) {
		S(i, i, 0, 0) = S_COL(0, i, 0, 0);
	}

	if (originalImage._spectrum == BLACK_AND_WHITE) {
		res = U * S * (V.transpose());
		res.save(newImgFile.c_str());
		printCompression(imgFile, newImgFile);
		std::remove(newImgFile.c_str());
	}
	else if (originalImage._spectrum == COLOR) {
		CImg<> U_tmp, tmp;
		for (int i = 0; i < COLOR; i++) {
			U_tmp = U.get_channel(i);
			tmp = U_tmp * S * (V.transpose());
			cimg_forXY(S, x, y) {
				res(x, y, 0, i) = tmp(x, y, 0, i);
			}
		}
	}

	CImgDisplay main_disp(originalImage, "Original Image");
	CImgDisplay compressed_disp(res, "Compressed Image");

	while(!main_disp.is_closed() || !compressed_disp.is_closed()) {
		CImgDisplay::wait(main_disp, compressed_disp);
	}

	return 0;
}

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
