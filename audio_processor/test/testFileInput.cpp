/**
 * @file testFileInput.cpp
 * 
 * @brief tests testFileInput class.
 * 
 * @author Joseph Kim.
 * 
 */

#include <iostream>
#include <assert.h>
#include <math.h>
#include "../inc/fileInput.hpp"

const char *TEST_FILE = "../../canary.wav";
const char *SOLN_FILE = "canaray.cso";
int main() {

	std::cout << "testing ... \n";

	FileInput fi(TEST_FILE);
 	FILE* f = fopen(SOLN_FILE, "r");

	std::cout << "number of frames " << fi.get_num_frames() << "\n";
	std::cout << "number of samplerate " << fi.get_samplerate() << "\n";

	int num_frames, samplerate;
	printf("Hello\n");
	fscanf(f, "%d\n%d\n", &samplerate, &num_frames);

	assert(fi.get_num_frames() == num_frames);
	assert(fi.get_samplerate() == samplerate);

	float curVal;
	for (int i = 0; i < num_frames; i++) {
		fscanf(f, "%f,", &curVal);
		assert(fabs(curVal - fi[i]) < 0.000001);
	}

	fclose(f);

	std::cout << "success!\n";
	return 0;
}