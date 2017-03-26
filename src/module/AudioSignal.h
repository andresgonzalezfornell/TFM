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
    int size;                               /**< number of samples */
    static float fs() { return 20000; }     /**< sampling frequency */
    AudioSignal();
	AudioSignal(vector<float> signal);
	virtual ~AudioSignal();
    float getSample(int index);
    void setSample(int index, float sample);
    void addSample(float sample);
	vector<float> getSignal();
    void setSignal(vector<float> signal);
    vector<float> getSpectrum();
    vector<float> getSpectrum(int bands);
    void clear();
private:
    vector<float> signal;                   /**< signal data vector */
};

#endif /* SRC_MODULE_AUDIOSIGNAL_H_ */
