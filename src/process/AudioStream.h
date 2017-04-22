#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

// System libraries
#include "math.h"
#include "stdlib.h"
#include "vector"
// Qt libraries
// Classes and local files
#include "../process/AudioSignal.h"
#include "../tools/Logger.h"

/**
 * @class	AudioStream
 * @author	Andrés González Fornell
 * @brief	Class for audio streams. This is the interface between the user interface and the coding and audio processing.
 */
class AudioStream {
public:
    /**
     * @brief   index range.
     */
    struct SignalRange {
        int start;                  /**< start index of the audio stream */
        int end;                    /**< end index of the audio stream */
    };
    SignalRange range;              /**< index range of the audio stream */
    AudioSignal signal;             /**< signal object */
    AudioStream(int fs);
    ~AudioStream();
    int getfs();
    void setfs(int value);
    void push(float sample);
    void push(AudioSignal sample);
    float pop();
    AudioSignal pop(int n);
    float getSample(int index);
    void setSample(int index, float sample);
    int size();
    bool isAvailable(int index);

private:
    int fs;                         /**< audio sampling frequency [Hz] */
};

#endif // AUDIOSTREAM_H
