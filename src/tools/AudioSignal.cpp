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
 * @param   fs              signal sampling frequency [Hz]
 */
AudioSignal::AudioSignal(int fs) {
    this->signal = std::vector<float>();
	this->size = 0;
    this->fs = fs;
}

/**
 * @brief	AudioSignal constructor.
 * @param   signal          vector of signal samples
 * @param   fs              signal sampling frequency [Hz]
 */
AudioSignal::AudioSignal(std::vector<float> signal, int fs) {
	this->signal = signal;
	this->size = this->signal.size();
    this->fs = fs;
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
 * @brief	It gets samples from a specific range.
 * @param   start           first index of the range (included)
 * @param   end             last index of the range (included)
 * @return  subsignal object
 */
AudioSignal AudioSignal::getSample(int start, int end) {
    std::vector<float>::const_iterator first = this->signal.begin() + start;
    std::vector<float>::const_iterator last = this->signal.begin() + end;
    std::vector<float> subsignal = std::vector<float>(first, last + 1);
    return AudioSignal(subsignal,this->fs);
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
    this->size++;
    // delete afterwards
    if(this->size > 100000) {
        this->clear();
    }
}

/**
 * @brief   It deletes a sample at a selected position
 * @param   index           sample position index
 */
void AudioSignal::deleteSample(int index) {
    this->signal.erase(this->signal.begin()+index);
    this->size--;
}

/**
 * @brief   It deletes a range of samples.
 * @param   start           first index of the range (included)
 * @param   end             last index of the range (included)
 */
void AudioSignal::deleteSample(int start, int end) {
    this->signal.erase(this->signal.begin()+start, this->signal.begin()+end+1);
    this->size -= end - start + 1;
}

/**
 * @brief	It gets the entire signal.
 * @return  signal
 */
std::vector<float> AudioSignal::getSignal() {
	return this->signal;
}

/**
 * @brief	It sets the entire signal.
 * @param   signal
 */
void AudioSignal::setSignal(std::vector<float> signal) {
	this->signal = signal;
}

/**
 * @brief   It gets time [s] axis as a vector beggining at time t = 0 s
 * @return  time vector
 */
std::vector<float> AudioSignal::getTimes() {
    int N = this->size;
    std::vector<float> times = std::vector<float>(N);
    for (int n = 0; n<N; n++) {
        times[n] = n/this->fs;
    }
    return times;
}

/**
 * @brief   It gets time [s] axis as a vector beggining at a specific initial time
 * @param   initialtime     initial time [s]
 * @return  time vector
 */
std::vector<float> AudioSignal::getTimes(float initialtime) {
    int N = this->size;
    std::vector<float> times = std::vector<float>(N);
    for (int n = 0; n<N; n++) {
        times[n] = n/this->fs + initialtime;
    }
    return times;
}

/**
 * @brief	It gets the signal spectral density.
 * @return  signal spectral density
 *
 */
std::vector<float> AudioSignal::getSpectrum() {
    int F = (int)ceil(this->size / 2);
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
    std::vector<float> spectrum = std::vector<float>(F);
    for (int f = 0; f < F; f++) {
        spectrum[f] = (pow(x_f[f][real], 2) + pow(x_f[f][imag], 2)) / (this->size * this->fs);
	}
	return spectrum;
}

/**
 * @brief	It gets the signal spectral density.
 * @param   bands           number of frequency bands of the signal spectral density (if higher number than available has been requested, it returns as the highest number of frequency as possible)
 * @return  signal spectral density
 */
std::vector<float> AudioSignal::getSpectrum(int bands) {
    std::vector<float> spectrum = this->getSpectrum();
    int F = spectrum.size();
    if (F<=bands) {
        return spectrum;
    } else {
        std::vector<float> spectrum_shrunk = std::vector<float>(bands);
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
            // Band level arithmetic mean calculation
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
 * @brief   It gets frequencies [Hz] axis as a vector
 * @return  frequencies vector
 */
std::vector<float> AudioSignal::getFrequencies() {
    int F = (int)ceil(this->size / 2);
    std::vector<float> frequencies = std::vector<float>(F);
    for (int f = 0; f<F; f++) {
        frequencies[f] = (this->fs/2)*((float)f/F);
    }
    return frequencies;
}

/**
 * @brief   It gets frequencies [Hz] axis as a vector
 * @param   bands           number of frequency bands of the signal spectral density (if higher number than available has been requested, it returns as the highest number of frequency as possible)
 * @return  frequencies vector
 */
std::vector<float> AudioSignal::getFrequencies(int bands) {
    std::vector<float> frequencies = this->getFrequencies();
    int F = frequencies.size();
    if (F<=bands) {
        return frequencies;
    } else {
        std::vector<float> frequencies_shrunk = std::vector<float>(bands);
        int step = ceil(F/bands);
        int band = 0;
        for(int f = 0; f < F; f += step) {
            float band_frequency = 0;
            int substeps;
            if(f+step<=F) {
                substeps = f+step;
            } else {
                substeps = F;
            }
            // Band frequency arithmetic mean calculation
            for (int substep = f; substep < substeps; substep++) {
                band_frequency += frequencies[substep] / step;
            }
            frequencies_shrunk[band] = band_frequency;
            band++;
        }
        return frequencies_shrunk;
    }
}

/**
 * @brief	It removes all samples from the signal.
 */
void AudioSignal::clear() {
	this->signal.clear();
	this->size = 0;
}
