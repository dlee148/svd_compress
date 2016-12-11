// svd_compress //

A very simple image compression application that operates on singular value decomposition.

// Dependencies: //

CImg

// Usage: //

To compile:
make

To run:

The compression factor is some value between 1 and 99.

./svd_compress FILE_NAME COMPRESSION_FACTOR
./svd_compress imgfile.pgm 50

// Notes: //
Due to limitations with CImg, usable file types at the moment are exceptionally limited.
