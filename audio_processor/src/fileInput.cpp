/**
 * @file fileInput.cpp
 * 
 * @brief conatins the implementation for FileInput, which parses and manages
 * audio files.
 * 
 * @author Joseph Kim
 * 
 */

#include "../inc/fileInput.hpp"
#include <sndfile.h>
#include <sndfile.hh>

FileInput::FileInput(const char *filename) {

	SndfileHandle file;
	file = SndfileHandle(filename);

	num_frames = file.frames();
	samplerate = file.samplerate();

	frames.reserve(num_frames);
	file.read(&frames.front(), num_frames);

}

void FileInput::save(const char *filename) {
	
	FILE* outfile = fopen(filename, "w");

	fprintf(outfile, "%d\n%d\n", samplerate, num_frames);

	for (auto it = frames.begin(); it < frames.end(); it++) {
		fprintf(outfile, "%f,", *it);
	}

	fclose(outfile);
}