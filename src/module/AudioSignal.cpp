#include "AudioSignal.h"

/**
 * @name    Constants references for complex numbers
 * @brief   It can be used to reference real and imaginary parts of a complex number
 * @{
 */
#define real 0
#define imag 1
/**
 * @}
 */


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
        consolelog("AudioSignal", LogType::error,
				"sample index exceeds the signal size");
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
    int F = (int)(this->size/2);
    fftw_complex x_t[this->size];
    fftw_complex x_f[this->size];
	for (int n = 0; n < this->size; n++) {
        x_t[n][real] = this->signal[n];
        x_t[n][imag] = 0;
	}
    fftw_plan fft = fftw_plan_dft_1d(this->size, x_t,
            x_f, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(fft);
	fftw_destroy_plan(fft);
	fftw_cleanup();
    vector<float> spectrum = vector<float>(F);
    for (int f = 0; f < F; f++) {
        spectrum[f] = (pow(x_f[f][real], 2) + pow(x_f[f][imag], 2)) / (this->size / 2) / fs();
	}
	return spectrum;
}

/**
 * @brief	It gets the signal spectral density.
 * @param   bands       number of frequency bands of the signal spectral density (if higher number than available has been requested, it returns as the highest number of frequency as possible)
 * @return  signal spectral density
 */
vector<float> AudioSignal::getSpectrum(int bands) {
    vector<float> spectrum = this->getSpectrum();
    int F = spectrum.size();
    if (F<=bands) {
        return spectrum;
    } else {
        vector<float> spectrum_shrunk = vector<float>(bands);
        int step = ceil(F/bands);
        int band = 0;
        for(int f = 0; f < F; f += step) {
            float band_level = 0;
            int substeps;
            if(f+step<=F) {
                substeps = f+step;
            } else {
                substeps = F;
            }
            for (int substep = f; substep < substeps; substep++) {
                band_level += spectrum[substep] / step;
            }
            spectrum_shrunk[band] = band_level;
            band++;
        }
        return spectrum_shrunk;
    }
}

/**
 * @brief	It removes all samples from the signal.
 */
void AudioSignal::clear() {
	this->signal.clear();
	this->size = 0;
}
