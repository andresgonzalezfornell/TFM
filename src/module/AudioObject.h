#ifndef SRC_MODULE_AUDIOOBJECT_H_
#define SRC_MODULE_AUDIOOBJECT_H_

// System libraries
#include "vector"
// Classes and local files
#include "../tools/Logger.h"

using namespace std;

/**
 * @class	AudioObject
 * @author	Andrés González Fornell
 * @brief	Audio objects for the SAOC interface.
 */
class AudioObject {
public:
    struct TimeSlot {
        int start, end;
    };
    TimeSlot timestamp;       /**< corresponding timestamp for the available data */
    AudioObject();
    ~AudioObject();
    void push(float sample);
    float pop();
    vector<float> getSamples();
    void setSample(int time, float sample);
    float getSample(int time);
    bool isAvailable(int time);
private:
    vector<float> samples;  /**< available data from the audio object */
};

#endif /* SRC_MODULE_AUDIOOBJECT_H_ */
