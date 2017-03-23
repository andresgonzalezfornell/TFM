#ifndef SRC_MODULE_AUDIOOBJECT_H_
#define SRC_MODULE_AUDIOOBJECT_H_

// System libraries
#include "vector"
// Classes and local files
#include "../tools/Logger.h"

namespace std {

class AudioObject {
public:
    AudioObject();
    ~AudioObject();
    int time_first;
    int time_last;
    void push(float sample);
    float pop();
    vector<float> getSamples();
    void setSample(int time, float sample);
    float getSample(int time);
    bool isAvailable(int time);
private:
    vector<float> samples;
};

} /* namespace std */

#endif /* SRC_MODULE_AUDIOOBJECT_H_ */
