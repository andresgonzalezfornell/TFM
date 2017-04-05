#include "AudioObject.h"

/**
 * @brief	AudioObject constructor.
 */
AudioObject::AudioObject() {
    this->timestamp.start = 0;
    this->timestamp.end = 0;
    consolelog("AudioObject", LogType::progress, "AudioObject object is created");
}

/**
 * @brief	AudioObject constructor.
 * @param   fs          signal sampling frequency [Hz]
 */
AudioObject::AudioObject(int fs) {
    this->fs = fs;
    this->timestamp.start = 0;
    this->timestamp.end = 0;
    consolelog("AudioObject", LogType::progress, "AudioObject object is created");
}

/**
 * @brief	AudioObject destructor.
 */
AudioObject::~AudioObject() {
    consolelog("AudioObject", LogType::progress, "AudioInput object is deleted");
}

/**
 * @brief	It adds a new sample to the end of the queu.
 * @param   sample
 */
void AudioObject::push(float sample) {
	this->samples.push_back(sample);
    this->timestamp.end++;
}

/**
 * @brief	It gets the first sample and deletes it from the queu.
 * @return	first sample
 */
float AudioObject::pop() {
	float sample = samples[1];
	this->samples.erase(samples.begin());
    this->timestamp.start++;
	return sample;
}

/**
 * @brief	It gets the samples vector.
 * @return	samples vector
 */
std::vector<float> AudioObject::getSamples() {
	return this->samples;
}

/**
 * @brief	It gets a sample from a selected time.
 * @param   time
 * @return	sample
 */
float AudioObject::getSample(int time) {
	if (this->isAvailable(time)) {
		return this->samples[time];
	} else {
        consolelog("AudioObject", LogType::error,
				"the selected time is not available in the audio object queu");
        return 0;
	}
}

/**
 * @brief	It sets a sample to a selected time.
 * @param   time
 * @param	sample
 */
void AudioObject::setSample(int time, float sample) {
	if (this->isAvailable(time)) {
		this->samples[time] = sample;
	} else {
        consolelog("AudioObject", LogType::error,
				"the selected time is not available in the audio object queu");
	}
}

/**
 * @brief	It checks if the selected time is available in the queu.
 * @param   time
 * @return  true if it is available
 */
bool AudioObject::isAvailable(int time) {
    return (this->timestamp.start <= time) && (this->timestamp.end >= time);
}
