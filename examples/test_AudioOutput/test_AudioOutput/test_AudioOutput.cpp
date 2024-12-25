#include <stdio.h>
#include <stdlib.h>
#include <ryulib/AudioIO.hpp>

const int CHANNEL_ = 1;
const int SAMPLE_RATE_ = 48000;
const int SAMPLE_SIZE_ = 4;
const int FRAMES_ = 5760;

int main()
{
	Audio::init();

	AudioOutput audio_output(CHANNEL_, SAMPLE_RATE_, SAMPLE_SIZE_, FRAMES_);
	audio_output.open(-1);

	AudioInput audio_input(CHANNEL_, SAMPLE_RATE_, SAMPLE_SIZE_, FRAMES_);
	audio_input.setOnData([&](const void* obj, const void* data, int size) {
		printf("size: %d \n", size);
		audio_output.play(data, size);
	});
	audio_input.open(-1);

	while (audio_input.isActive()) {
		Pa_Sleep(1000);
	}

	audio_input.close();
}
