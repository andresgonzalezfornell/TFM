#ifndef SRC_MODULE_AUDIOSIGNAL_H_
#define SRC_MODULE_AUDIOSIGNAL_H_

// System libraries
#include "vector"
#include "cmath"
// External libraries
#include "fftw3.h"
// Classes and local files
#include "../tools/Logger.h"

/**
 * @class	AudioSignal
 * @author	Andrés González Fornell
 * @brief	TODO AudioSignal.cpp description
 */
class AudioSignal {
public:
    int size; /**< number of samples */
    int fs; /**< signal sampling frequency [Hz] */
    static const unsigned int maxsamples = 0xFFFFFF; /**< maximum number of samples */
    AudioSignal(int fs);
    AudioSignal(std::vector<float> signal, int fs);
    ~AudioSignal();
    float operator[](int index);
    AudioSignal getSample(int start, int end);
    void setSample(int index, float sample);
    void addSample(float sample);
    void deleteSample(int index);
    void deleteSample(int start, int end);
    std::vector<float> getSignal();
    void setSignal(std::vector<float> signal);
    std::vector<float> getTimes();
    std::vector<float> getTimes(float initialtime);
    std::vector<float> getSpectrum();
    std::vector<float> getSpectrum(int bands);
    std::vector<float> getFrequencies();
    std::vector<float> getFrequencies(int bands);
    void clear();
private:
    std::vector<float> signal; /**< signal data vector */
};

#endif /* SRC_MODULE_AUDIOSIGNAL_H_ */
