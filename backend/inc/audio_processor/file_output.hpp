/**
 * @file file_output.hpp
 * 
 * @brief conatins the API for FileInput, which parses and manages
 * audio files.
 * 
 * @author Joseph Kim
 * 
 */

#ifndef _FILEOUTPUT_H_
#define _FILEOUTPUT_H_

#include <vector>

/**
 * @brief Class for file inputs.
 *
 */
class FileOutput
{
public:
	/* cosntructor that takes in a filename and processes it */
	FileOutput(const char *filename, int samplerate);

	/** @brief Defines iterator type over lines in the netlist */
    typedef std::vector<float>::iterator iterator;
    /** @brief Defines const iterator type over lines in the netlist */
    typedef std::vector<float>::const_iterator const_iterator;

    /** @brief returns an iterator to the beginning of the audio data */
    iterator begin() { return frames.begin(); }
    /** @brief returns an iterator to the end of the audio data */
    iterator end() { return frames.end(); }
    /** @brief returns a constant iterator to the beginning of the audio data */
    const_iterator cbegin() { return frames.begin(); }
    /** @brief returns a constant iterator to the end of the audio data */
    const_iterator cend() { return frames.end(); }

    /** @brief returns the number of frames */
	int get_num_frames(){ return num_frames; };
	/** @brief returns the samplerate */
	int get_samplerate(){ return samplerate; };

	/** @brief returns the next voltage */
	void set_next_value(float val);

	/** @brief saves the file as a cso file */
	void finish();

	/** @brief define subscript operator */
	float &operator[](int i) {
		return frames[i];
	}

private:
	/** @brief name of the file to create */
	const char *filename;
	/** @brief vector containing frame data */
	std::vector<float> frames;
	/** @brief samplerate of the file */
	int samplerate;
	/** @brief number of frames the file contains */
	int num_frames;
};

#endif /* _FILEINPUT_H_ */