/**
 * @file fileInput.hpp
 * 
 * @brief conatins the API for FileInput, which parses and manages
 * audio files.
 * 
 * @author Joseph Kim
 * 
 */

#ifndef _FILEINPUT_H_
#define _FILEINPUT_H_

#include <vector>
#include <input_interface.hpp>
#include <audio_manager.hpp>

/**
 * @brief Class for file inputs.
 *
 */
class FileInput : public InputInterface
{
public:
	/* cosntructor that takes in a filename and processes it */
	FileInput(const char *filename, AudioManager::filetype_t file_type);

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
	int get_samplerate() override { return samplerate; };

	/** @brief returns the next voltage */
	bool get_next_value(float *val) override;
	/** @brief resets the current index back to zero */
	void reset_index() { cur_index = 0; }

	/** @brief saves the file as a cso file */
	void save(const char* filename);

	/** @brief define subscript operator */
	float &operator[](int i) {
		return frames[i];
	}

private:
	/** @brief vector containing frame data */
	std::vector<float> frames;
	/** @brief samplerate of the file */
	int samplerate;
	/** @brief number of frames the file contains */
	int num_frames;
	/** @brief index get_next_value will serve */
	int cur_index;
	/** @brief the number of channels the input file had */
	int channels;
};

#endif /* _FILEINPUT_H_ */