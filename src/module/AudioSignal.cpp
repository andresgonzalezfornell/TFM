/**
 * @name	AudioSignal.cpp
 * @author	Andrés González Fornell
 * @brief	TODO AudioSignal.cpp description
 */

#include "AudioSignal.h"

/**
 * @brief	AudioSignal constructor (empty signal vector).
 */
AudioSignal::AudioSignal() {
    this->signal = vector<float>();
    this->size = 0;
}

/**
 * @brief	AudioSignal constructor.
 * @param   signal      vector of signal samples
 */
AudioSignal::AudioSignal(vector<float> signal) {
    this->signal = signal;
	this->size = this->signal.size();
}

/**
 * @brief	AudioSignal destructor.
 */
AudioSignal::~AudioSignal() {
}

/**
 * @brief	It gets a sample from the selected index.
 * @param   index
 * @return  sample
 */
float AudioSignal::getSample(int index) {
    return this->signal[index];
}

/**
 * @brief	It sets a sample in the selected index.
 * @param   index
 * @param   sample
 */
void AudioSignal::setSample(int index, float sample) {
    if (index >= this->size) {
        consolelog("AudioSignal",error,"sample index exceeds the signal size");
    } else {
        this->signal[index] = sample;
    }
}

/**
 * @brief	It adds a sample to the end of the signal.
 * @param   sample
 */
void AudioSignal::addSample(float sample) {
    this->signal.push_back(sample);
    this->size += 1;
}

/**
 * @brief	It gets the entire signal.
 * @return  signal
 */
vector<float> AudioSignal::getSignal() {
	return this->signal;
}

/**
 * @brief	It sets the entire signal.
 * @param   signal
 */
void AudioSignal::setSignal(vector<float> signal) {
    this->signal = signal;
}

/**
 * @brief	It gets the signal spectral density.
 * @return  signal spectral density
 */
vector<float> AudioSignal::getSpectrum() {
    CArray complexsignal(Complex(), this->size);
    for (int index = 0; index < this->size; index++) {
        complexsignal[index] = Complex(this->signal[index],0);
    }
    CArray complexspectrum = fft(complexsignal);
    vector<float> spectrum = vector<float>(this->size);
    for (int index = 0; index < this->size; index++) {
        spectrum[index] = (float)complexspectrum[index].real();
    }
    return spectrum;
}

/**
 * @brief	It removes all samples from the signal.
 */
void AudioSignal::clear() {
    this->signal.clear();
    this->size = 0;
}
