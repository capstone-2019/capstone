#include <stdio.h>
#include <iostream>
#include <sndfile.h>
#include <algorithm>

#include	<cstdio>
#include	<cstring>
#include	<sndfile.hh>

#define BUF_SIZE 1024

SndfileHandle open_file(const char* filename) {

	SndfileHandle file;

	file = SndfileHandle(filename);

	printf("opened %s. %lld frames at a samplerate of %d\n", 
			filename,
			file.frames(), 
			file.samplerate());


	return file;
}




int main(int argc, char* argv[]) {

	/* check args */
	if (argc == 1) {
		printf("please provide a input filename and an output filename\n");
		return -1;
	} else if (argc > 3) {
		printf("too many arguments\n");
		return -2;
	}

	SndfileHandle file = open_file(argv[1]);

	FILE* outfile = fopen(argv[2], "w");
	int frames = (int) file.frames();

	fprintf(outfile, "%d\n%d\n", file.samplerate(), frames);

	float buf[BUF_SIZE];
	for (int i = 0; i < frames; i += BUF_SIZE) {
		file.read(buf, BUF_SIZE);

		for (int j = 0; j < BUF_SIZE && i+j < frames; j++) {
			fprintf(outfile, "%f,", buf[j]);
		}
	}

}