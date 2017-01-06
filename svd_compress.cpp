#include "utilities.h"
#include "constants.h"
#include "CImg.h"
using namespace cimg_library;

int main(int argc, char** argv) {
	cimg_library::cimg::exception_mode(0);

	if (argc != 3) {
		throwError(USAGE_ERROR);
	}

	std::string imgFile = argv[1];
	std::string newImgFile = formatTmpImgPath(argv[1]).c_str();
	int compressionFactor = atoi(argv[2]);

	if (compressionFactor > 99 || compressionFactor < 0) {
		throwError(COMPRESSION_FACTOR_ERROR);
	}

	CImg<unsigned char> originalImage;

	try {
		originalImage.load(imgFile.c_str());
	}
	catch (CImgException& e) {
		throwError(FILE_NOT_FOUND_ERROR);
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
	}
	else if (originalImage._spectrum == COLOR) {
		res.assign(originalImage._width, originalImage._height, 1, 3);
		CImg<> U_tmp, tmp;
		for (int i = 0; i < COLOR; i++) {
			U_tmp = U.get_channel(i);
			tmp = U_tmp * S * (V.transpose());
			cimg_forXY(tmp, x, y) {
				res(x, y, 0, i) = tmp(x, y, 0, 0);
			}
		}
	}
	
	res.save(newImgFile.c_str());
	printCompression(imgFile, newImgFile);
	std::remove(newImgFile.c_str());

	CImgDisplay main_disp(originalImage, "Original Image");
	CImgDisplay compressed_disp(res, "Compressed Image");

	while(!main_disp.is_closed() || !compressed_disp.is_closed()) {
		CImgDisplay::wait(main_disp, compressed_disp);
	}

	return 0;
}
