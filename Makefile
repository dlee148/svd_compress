svd_compress: svd_compress.cpp 
	g++ -o $@ svd_compress.cpp utilities.cpp -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11

clean:
	rm -f svd_compress
