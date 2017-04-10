#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

// System libraries
#include "math.h"
#include "stdlib.h"
#include "vector"
// Qt libraries
// Classes and local files
#include "../tools/Logger.h"

/**
 * @class	AudioStream
 * @author	Andrés González Fornell
 * @brief	Class for audio streams. This is the interface between the user interface and the coding and audio processing.
 */
class AudioStream {
public:
    /**
     * @brief   Time range.
     */
    struct TimeRange {
        int start;                  /**< start time of the audio stream */
        int end;                    /**< end time of the audio stream */
    };
    TimeRange timerange;            /**< time range of the audio stream */
    std::vector<float> samples;     /**< signal samples */
    AudioStream(int fs);
    ~AudioStream();
    void push(float sample);
    float pop();
    std::vector<float> getSamples();
    float getSample(int time);
    void setSample(int time, float sample);
    bool isAvailable(int time);
    int getfs();
    void setfs(int value);

private:
    int fs;                         /**< audio sampling frequency [Hz] */
};

#endif // AUDIOSTREAM_H
