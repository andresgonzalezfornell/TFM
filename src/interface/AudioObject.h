#ifndef SRC_MODULE_AUDIOSTREAM_H_
#define SRC_MODULE_AUDIOSTREAM_H_

// System libraries
#include "vector"
// Classes and local files
#include "../tools/Logger.h"

/**
 * @class	AudioStream
 * @author	Andrés González Fornell
 * @brief	Audio objects for the SAOC interface.
 */
class AudioStream {
public:
    /**
     * @brief   It indicates time slot of the available signal.
     */
    struct TimeSlot {
        int start;          /**< start time */
        int end;            /**< end time */
    };
    TimeSlot timestamp;     /**< corresponding timestamp for the available data */
    int fs;               /**< signal sampling frequency [Hz] */
    AudioStream();
    AudioStream(int fs);
    ~AudioStream();
    void push(float sample);
    float pop();
    std::vector<float> getSamples();
    void setSample(int time, float sample);
    float getSample(int time);
    bool isAvailable(int time);
private:
    std::vector<float> samples;  /**< available data from the audio object */
};

#endif /* SRC_MODULE_AUDIOSTREAM_H_ */
