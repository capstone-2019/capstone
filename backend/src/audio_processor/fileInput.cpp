/**
 * @file fileInput.cpp
 * 
 * @brief conatins the implementation for FileInput, which parses and manages
 * audio files.
 * 
 * @author Joseph Kim
 * 
 */

#include <fileInput.hpp>
#include <sndfile.h>
#include <sndfile.hh>
#include <iostream>
#include <fstream>
#include <string>

using std::ifstream;
using std::string;

FileInput::FileInput(const char *filename, AudioManager::filetype_t file_type) {

	if (file_type == AudioManager::FILETYPE_WAV) {
		SndfileHandle file;
		file = SndfileHandle(filename);

		num_frames = file.frames();
		samplerate = file.samplerate();

		frames.resize(num_frames);
		file.read(&frames.front(), num_frames);

		cur_index = 0;
	} else if (file_type == AudioManager::FILETYPE_TXT) {
		ifstream file(filename);
		string line;

		getline(file, line);
		samplerate = (int) (1.f / stod(line));

		while (getline(file, line)) {
			frames.push_back(stod(line));
		}
		num_frames = frames.size();
	}
	

}

void FileInput::save(const char *filename) {
	
	FILE* outfile = fopen(filename, "w");

	fprintf(outfile, "%d\n%d\n", samplerate, num_frames);

	for (auto it = frames.begin(); it < frames.end(); it++) {
		fprintf(outfile, "%f,", *it);
	}

	fclose(outfile);
}

bool FileInput::get_next_value(float *val) {
	if (cur_index >= num_frames) {
		return false;
	}
	*val = frames[cur_index++];
	return true;
}




