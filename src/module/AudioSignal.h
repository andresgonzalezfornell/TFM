#ifndef SRC_MODULE_AUDIOSIGNAL_H_
#define SRC_MODULE_AUDIOSIGNAL_H_

// System libraries
#include "vector"
#include "math.h"
#include "valarray"
// Classes and local files
#include "../tools/Logger.h"
#include "../tools/fft.h"

using namespace std;

typedef std::valarray<Complex> CArray;

class AudioSignal {
public:
    int size;
    static float fs() { return 20000; }
    AudioSignal();
	AudioSignal(vector<float> signal);
	virtual ~AudioSignal();
    float getSample(int index);
    void setSample(int index, float sample);
    void addSample(float sample);
	vector<float> getSignal();
    void setSignal(vector<float> signal);
    vector<float> getSpectrum();
    void clear();
private:
	vector<float> signal;
};

#endif /* SRC_MODULE_AUDIOSIGNAL_H_ */
