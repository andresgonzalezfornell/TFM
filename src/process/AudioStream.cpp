#include "AudioStream.h"

/**
 * @brief	AudioStream constructor.
 * @param   fs          signal sampling frequency [Hz]
 */
AudioStream::AudioStream(int fs) : signal(fs) {
    this->fs = fs;
    this->range.start = 0;
    this->range.end = -1;
}

/**
 * @brief	AudioStream destructor.
 */
AudioStream::~AudioStream() {
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
 * @brief	It adds a new sample to the end of the stream.
 * @param   sample
 */
void AudioStream::push(float sample) {
    this->signal.addSample(sample);
    this->range.end++;
}

/**
 * @brief	It adds several new samples to the end of the stream.
 * @param   samples
 */
void AudioStream::push(AudioSignal samples) {
    for(int index = 0; index < samples.size; index++) {
        this->push(samples[index]);
    }
}

/**
 * @brief	It gets the first sample and deletes it from the stream.
 * @return	first sample
 */
float AudioStream::pop() {
    if (this->size() == 0) {
        consolelog("AudioStream",LogType::error,"audio stream object is empty so zero value is returned");
        return 0;
    } else {
        float sample = this->signal[0];
        this->signal.deleteSample(0);
        this->range.start++;
        return sample;
    }
}

/**
 * @brief	It gets the first n samples and deletes them from the stream.
 * @param   n           number of samples to pop from the stream
 * @return	first n samples
 */
AudioSignal AudioStream::pop(int n) {
    if (n > this->size()) {
        n = this->size();
        consolelog("AudioStream",LogType::warning,"not enough available samples so all samples (" + std::to_string(n) + ") are taken");
    }
    AudioSignal subsignal = this->signal.getSample(0,n-1);
    this->signal.deleteSample(0,n-1);
    this->range.start += n;
    return subsignal;
}

/**
 * @brief	It gets a sample from a selected index (the sample is not deleted).
 * @param   index
 * @return	sample
 */
float AudioStream::operator[](int index) {
    if (this->isAvailable(index)) {
        return this->signal[index];
    } else {
        consolelog("AudioStream", LogType::error,
                "the selected index (" + std::to_string(index) + ") is not available in the audio object stream");
        return 0;
    }
}

/**
 * @brief	It sets a sample to a selected index (it replaces the previous sample at the same selected index).
 * @param   index
 * @param	sample
 */
void AudioStream::setSample(int index, float sample) {
    if (this->isAvailable(index)) {
        this->signal.setSample(sample,index);
	} else {
        consolelog("AudioStream", LogType::error,
                "the selected index (" + std::to_string(index) + ") is not available in the audio object stream");
	}
}

/**
 * @brief   It gets the number of samples.
 * @return  number of samples of audio stream
 */
int AudioStream::size() {
    if (this->range.end - this->range.start + 1 < 0) {
        this->range.end = this->range.start - 1;
        consolelog("AudioStream",LogType::warning,"samples range is invalid so it has been fixed by changing end index to " + std::to_string(this->range.end));
    }
    return this->range.end - this->range.start + 1;
}

/**
 * @brief	It checks if the selected index is available in the stream.
 * @param   index
 * @return  true if it is available
 */
bool AudioStream::isAvailable(int index) {
    return (this->range.start <= index) && (this->range.end >= index);
}
