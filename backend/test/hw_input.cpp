


#include <audio_manager.hpp>
#include <iostream>


#define TEST_LEN (44100*4)

using namespace std;

int main() {
	AudioManager *am = new AudioManager(AudioManager::INPUT_HARDWARE,
										AudioManager::OUTPUT_FILE | AudioManager::OUTPUT_HARDWARE,
										NULL,
										"test.txt",
										AudioManager::FILETYPE_TXT);
	cout << "starting test...\n";

	double val;
	for (int i = 0; i < TEST_LEN; i++) {
		cout << "\titeration " << i << ". " << flush;
		am->get_next_value(&val);
		cout << ". " << flush;
		am->set_next_value(val);
		cout << ". done.\n" << flush;
	}

	cout << "finishing..." << flush;
	am->finish();
	cout << " done.\n" << flush;

	return 0;
}