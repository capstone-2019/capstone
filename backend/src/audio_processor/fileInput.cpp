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

		channels = file.channels();
		frames.resize(num_frames*channels);
		int n = file.read(&frames.front(), channels*num_frames);

		// std::cout << "n " << n << "\n";
		// std::cout << "channels " << channels << "\n";

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
		cur_index = 0;
		channels = 1;
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
	float tmp = 0.f;
	float inv = 1.f / channels;
	for (int i = 0; i < channels; i++) {
		tmp += inv * frames[cur_index++];
	}
	*val = tmp;
	return true;
}




