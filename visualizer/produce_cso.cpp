/* @file produce_cso.cpp
 *
 * @brief Takes in a file and converts it into a .cso file.
 * 
 * a .cso file is a mono audio file represented in the following format:
 *
 * samplerate\n
 * framerate\n
 * comma separated list of each sample represented as floats.
 * 
 * Use:
 * 
 * ./produce_cso INPUT_FILE_NAME OUTPUT_FILE_NAME/
 *
 * Currently only works on sound formats libsndfile supports, such as wav.
 *
 * @author Joseph Kim
 *
 */

#include <stdio.h>
#include <unistd.h>

#include <sndfile.h>
#include <sndfile.hh>

/* size of the buffer to be used to copy over data. */
#define BUF_SIZE 1024


/*
 * @brief Opens an audio file as an sndfile handle.
 * Also reports the file read, number of frames and the sample rate.
 * The SndfileHandle is a C++ wrapper around Sndfile.
 *
 * @param filename the name of the file to be opened.
 * 
 * @return an file handle for the given file.
 */
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
	} else if (access(argv[2], F_OK) != -1) {
		printf("File already exists... Continue? (y/n) \n");
		char c;
		do {
			c = getchar();
			if (c == 'n') {
				printf("aborting.\n");
				return -3;
			} else if (c != 'y') {
				printf("invalid option.\n");
			}
		} while(c != 'y');
		
	}

	SndfileHandle file = open_file(argv[1]);
	FILE* outfile = fopen(argv[2], "w");

	int frames = (int) file.frames();

	/* write header information to the new file */
	fprintf(outfile, "%d\n%d\n", file.samplerate(), frames);

	/* write in comman separated list */
	float buf[BUF_SIZE];
	for (int i = 0; i < frames; i += BUF_SIZE) {
		file.read(buf, BUF_SIZE);
		for (int j = 0; j < BUF_SIZE && i+j < frames; j++) {
			fprintf(outfile, "%f,", buf[j]);
		}
	}
}