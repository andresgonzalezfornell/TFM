#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

// System libraries
#include "stdlib.h"
#include "cstring"
// Classes and local files
#include "File.h"
#include "../effects/Effect.h"
#include "../tools/Logger.h"
#include "../sac/SACBitstream.h"
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
    int fs;                 /**< signal sampling frequency */
    float **input;          /**< vector of input channels stream (sample = input[channel][sample index]) */
    float **output;         /**< vector of input channels stream (sample = output[channel][sample index]) */
    int channels;           /**< number of channels */
    int samples;            /**< number of samples in each channel */
    int cursor;             /**< pointer to current sample index when executing real time process*/
    int total;              /**< number of available output samples */
    // SAOC parameters
    float **NRG;            /**< object energies (NRG[channel][chunk]) */
    float **OLD;            /**< object level differences (OLD[channel][chunk]) */
    ProcessManager(int chunksize);
    ~ProcessManager();
    bool setInput(std::string filename);
    bool setOutput(std::string filename);
	bool decode(std::string input, std::string bitstream, std::string output,
			int upmixtype, int decodingtype, int binauralquality,
			int hrtfmodel);
    bool applyEffect(Effect *effect, std::vector<bool> channels,
            std::vector<double> levels);
	void clear();
private:
    SACBitstream *bitstream; /**< bitstream object */
    WAVFile *inputfile;     /**< audio input file object */
    WAVFile *outputfile;    /**< audio output file object */
    bool allocated;         /**< true if input and output signals variables are currently allocated */
    int chunksize;          /**< number of samples in a chunk */
};

#endif // PROCESSMANAGER_H
