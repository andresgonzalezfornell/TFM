#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

// System libraries
#include "stdlib.h"
// Classes and local files
#include "../process/AudioStream.h"
#include "../effects/Effect.h"
#include "../tools/Logger.h"
extern "C" {
#include "../sac/sac_decoder.h"
}

/**
 * @class	ProcessManager
 * @author	Andrés González Fornell
 * @brief	Process manager class. It contains all functions to perform the signal treatment process.
 */
class ProcessManager {
public:
    std::vector<AudioStream> input;     /**< vector of input channels stream */
    std::vector<AudioStream> output;    /**< vector of input channels stream */
    ProcessManager(int fs, int chunksize);
    ~ProcessManager();
    void setInput(/*AudioFile file*/);
    void setOutput(int channels);
    bool decode(std::string input, std::string bitstream, std::string output, int decodingtype, int upmixtype, int binauralquality, int hrtfmodel);
    bool applyEffect(std::vector<bool> channels, Effect effect);
private:
    int fs;                             /**< signal sampling frequency */
    int chunksize;                      /**< number of samples in a chunk */
};

#endif // PROCESSMANAGER_H
