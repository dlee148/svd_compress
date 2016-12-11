#include <iostream>
#include <string>
#include "CImg.h"
using namespace cimg_library;

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cout << "Usage: ./svd_compress IMAGE_FILE COMPRESSION_FACTOR (Percentage)" << std::endl;
		return 0;
	}

	char* imgFile = argv[1];
	int compressionFactor = atoi(argv[2]);
	
	if (compressionFactor > 99 || compressionFactor < 0) {
		std::cout << "Compression factor must be between 0-99." << std::endl;
		return 0;
	}
	
	const CImg<unsigned char> originalImage = CImg<>(imgFile);
	compressionFactor = floor((((double)compressionFactor / (double)100) * originalImage._height));
	
	CImg<> U, S_COL, V, res;
	originalImage.SVD(U, S_COL, V);
	CImg<> S(U._height, U._width, 1, 1);
	
	for (int i = 0; i < S_COL._height - compressionFactor; i++) {
		S(i, i, 0, 0) = S_COL(0, i, 0, 0);
	}
	
	res = U * S * (V.transpose());
	
	CImgDisplay main_disp(originalImage, "Original Image");
	CImgDisplay compressed_disp(res, "Compressed Image");
	
	while(true) {
		CImgDisplay::wait(main_disp, compressed_disp);
	}
	
	return 0;
}
