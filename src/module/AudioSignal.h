#ifndef SRC_MODULE_AUDIOSIGNAL_H_
#define SRC_MODULE_AUDIOSIGNAL_H_

// System libraries
#include "vector"
#include "math.h"
// External libraries
#include "fftw3.h"
// Classes and local files
#include "../tools/Logger.h"

using namespace std;

/**
 * @class	AudioSignal
 * @author	Andrés González Fornell
 * @brief	TODO AudioSignal.cpp description
 */
class AudioSignal {
public:
    int size;                       /**< number of samples */
    float fs;                       /**< sampling frequency [Hz] */
    AudioSignal(float fs);
    AudioSignal(vector<float> signal, float fs);
	virtual ~AudioSignal();
    float getSample(int index);
    void setSample(int index, float sample);
    void addSample(float sample);
	vector<float> getSignal();
    void setSignal(vector<float> signal);
    vector<float> getTimes();
    vector<float> getSpectrum();
    vector<float> getSpectrum(int bands);
    vector<float> getFrequencies();
    vector<float> getFrequencies(int bands);
    void clear();
private:
    vector<float> signal;           /**< signal data vector */
};

#endif /* SRC_MODULE_AUDIOSIGNAL_H_ */
