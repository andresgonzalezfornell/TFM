/**
 * @name	test.cpp
 * @author	Andrés González Fornell
 * @brief	Test Unit file.
 */

// Functional libraries
#include "tools/Logger.h"
#include "tools/AudioStream.h"

int main(int argc, char *argv[]) {
	consolelog("test", LogType::progress, "running test");

	int fs = 44100;
	AudioStream stream = AudioStream(fs);
	stream.push(1);
    AudioSignal signal = AudioSignal(fs);
	signal.addSample(2);
	signal.addSample(3);
    signal.addSample(4);
	stream.push(signal);
	float value1 = stream.pop();
    AudioSignal s = stream.pop(3);

	consolelog("test", LogType::progress, "test was completed");
	return 0;
}
