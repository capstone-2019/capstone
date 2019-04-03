/**
 * @file file_output.cpp
 * 
 * @brief conatins the implementation for FileOutput, which stores and saves
 * audio data.
 * 
 * @author Joseph Kim
 * 
 */

#include <file_output.hpp>
#include <iostream>
#include <fstream>

/**
 * @brief Constructs the FileOutput object.
 *
 * @param filename The filename to create.
 * @param samplerate The samplerate of the audio data.
 */
FileOutput::FileOutput(const char *filename, int samplerate) {

	this->filename = filename;
	this->samplerate = samplerate;
	this->num_frames = 0;

}

void FileOutput::set_next_value(float val) {
	frames.push_back(val);
	num_frames++;
}

void FileOutput::finish() {

	std::ofstream outfile (filename, std::ios::out);

	if (outfile.is_open()) {
		outfile << samplerate << "\n";
		outfile << num_frames << "\n";

		for (int i = 0; i < num_frames; i++) {
			outfile << frames[i] << ",";
		}

	} else {
		std::cout << "unable to open file.\n";
	}

}