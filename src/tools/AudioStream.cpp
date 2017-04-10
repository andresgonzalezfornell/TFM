#include "AudioStream.h"

/**
 * @brief	AudioStream constructor.
 * @param   fs          signal sampling frequency [Hz]
 */
AudioStream::AudioStream(int fs) {
    this->fs = fs;
    this->timerange.start = 0;
    this->timerange.end = 0;
    consolelog("AudioStream", LogType::progress, "AudioStream object is created");
}

/**
 * @brief	AudioStream destructor.
 */
AudioStream::~AudioStream() {
    consolelog("AudioStream", LogType::progress, "AudioInput object is deleted");
}

/**
 * @brief   It gets the signal sampling frequency.
 * @return  signal sampling frequency
 */
int AudioStream::getfs() {
    return this->fs;
}

/**
 * @brief   It sets the signal sampling frequency.
 * @param   fs          signal sampling frequency
 */
void AudioStream::setfs(int fs) {
    this->fs = fs;
}

/**
 * @brief	It adds a new sample to the end of the queu.
 * @param   sample
 */
void AudioStream::push(float sample) {
	this->samples.push_back(sample);
    this->timerange.end++;
}

/**
 * @brief	It gets the first sample and deletes it from the queu.
 * @return	first sample
 */
float AudioStream::pop() {
	float sample = samples[1];
	this->samples.erase(samples.begin());
    this->timerange.start++;
	return sample;
}

/**
 * @brief	It gets the samples vector.
 * @return	samples vector
 */
std::vector<float> AudioStream::getSamples() {
	return this->samples;
}

/**
 * @brief	It gets a sample from a selected time.
 * @param   time
 * @return	sample
 */
float AudioStream::getSample(int time) {
    if (this->isAvailable(time)) {
        return this->samples[time];
    } else {
        consolelog("AudioStream", LogType::error,
                "the selected time is not available in the audio object queu");
        return 0;
    }
}

/**
 * @brief	It sets a sample to a selected time.
 * @param   time
 * @param	sample
 */
void AudioStream::setSample(int time, float sample) {
	if (this->isAvailable(time)) {
		this->samples[time] = sample;
	} else {
        consolelog("AudioStream", LogType::error,
				"the selected time is not available in the audio object queu");
	}
}

/**
 * @brief	It checks if the selected time is available in the queu.
 * @param   time
 * @return  true if it is available
 */
bool AudioStream::isAvailable(int time) {
    return (this->timerange.start <= time) && (this->timerange.end >= time);
}
