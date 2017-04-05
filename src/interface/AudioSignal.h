#ifndef SRC_MODULE_AUDIOSIGNAL_H_
#define SRC_MODULE_AUDIOSIGNAL_H_

// System libraries
#include "vector"
#include "math.h"
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
    int size;                       /**< number of samples */
    int fs;                       /**< signal sampling frequency [Hz] */
    AudioSignal(int fs);
    AudioSignal(std::vector<float> signal, int fs);
	virtual ~AudioSignal();
    float getSample(int index);
    void setSample(int index, float sample);
    void addSample(float sample);
    std::vector<float> getSignal();
    void setSignal(std::vector<float> signal);
    std::vector<float> getTimes();
    std::vector<float> getSpectrum();
    std::vector<float> getSpectrum(int bands);
    std::vector<float> getFrequencies();
    std::vector<float> getFrequencies(int bands);
    void clear();
private:
    std::vector<float> signal;           /**< signal data vector */
};

#endif /* SRC_MODULE_AUDIOSIGNAL_H_ */
