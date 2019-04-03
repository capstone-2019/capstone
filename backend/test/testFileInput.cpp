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

#define EPSILON 0.000001

const char *TEST_FILE = "../../canary.wav";
const char *SOLN_FILE = "canaray.cso";

int main() {

	std::cout << "testing ... \n";

	FileInput fi(TEST_FILE);
 	FILE* f = fopen(SOLN_FILE, "r");

	std::cout << "number of frames " << fi.get_num_frames() << "\n";
	std::cout << "number of samplerate " << fi.get_samplerate() << "\n";

	int num_frames, samplerate;
	fscanf(f, "%d\n%d\n", &samplerate, &num_frames);

	assert(fi.get_num_frames() == num_frames);
	assert(fi.get_samplerate() == samplerate);

	/* test all ways of getting data */
	float soln_val, test_val;
	int i = 0;
	auto xxx = fi.end();

	for (auto it = fi.begin(); it < fi.end(); it++) {
		fscanf(f, "%f,", &soln_val);
		assert(fi.get_next_value(&test_val));
		assert(fabs(soln_val - fi[i++]) < EPSILON);
		assert(fabs(soln_val - test_val) < EPSILON);
		assert(fabs(soln_val - *it) < EPSILON);
	}
	std::cout << "hello\n";

	assert(!fi.get_next_value(NULL));

	fclose(f);

	std::cout << "success!\n";
	return 0;
}