// Class libraries
#include "ProcessManager.h"

/**
 * @brief   ProcessManager constructor.
 * @param   fs                  signal sampling frequency
 * @param   chunksize           number of samples in a chunk
 */
ProcessManager::ProcessManager(int fs, int chunksize) {
    this->fs = fs;
    this->chunksize = chunksize;
    consolelog("ProcessManager",LogType::progress,"ProgressManager object is created");
}

/**
 * @brief   ProcessManager destructor.
 */
ProcessManager::~ProcessManager() {
    consolelog("ProcessManager",LogType::progress,"ProgressManager object is deleted");
}

/**
 * @brief   It sets input variable.
 * @param   file                file object to extract input variable
 */
void ProcessManager::setInput() {
    this->input.clear();
}

/**
 * @brief   It sets an empty output variable.
 * @param   channels            number of channels of the output variable
 */
void ProcessManager::setOutput(int channels) {
    this->output.clear();
    for (int channel = 0; channel < this->input.size(); channel++) {
        this->output.push_back(AudioStream(fs));
    }
}

/**
 * @brief   It performs the SAC encoder.
 * @param   input               filename of the multichannel input audio file
 * @param   outpu               filename of the downmix output audio file (it will be automatically created)
 * @param   bitstream           filename of the bitstream output file or "buried" (it will be automatically created)
 * @param   fs                  audio sampling frequency [Hz]
 * @param   tree                tree config: 5151 (mono), 5152 (mono), 525 (stereo) (5151 by default)
 * @param   timeslots           times slots: 16 or 32 (32 by default)
 * @return  true if it was successful
 */
bool ProcessManager::decode(std::string input, std::string bitstream, std::string output, int decodingtype, int upmixtype, int binauralquality, int hrtfmodel) {
    const char *input_char = input.c_str();
    const char *bitstream_char = bitstream.c_str();
    const char *output_char = output.c_str();
    consolelog("ProcessManager",LogType::info,"input file:\t\t " + input);
    consolelog("ProcessManager",LogType::info,"bitstream file:\t " + bitstream);
    consolelog("ProcessManager",LogType::info,"output file:\t\t " + output);
    consolelog("ProcessManager",LogType::info,"upmix type:\t " + std::to_string(upmixtype));
    consolelog("ProcessManager",LogType::info,"sampling frequency:\t " + std::to_string(this->fs) + "Hz");
    consolelog("ProcessManager",LogType::info,"upmix type:\t\t " + std::to_string(upmixtype));
    consolelog("ProcessManager",LogType::info,"decoding type:\t " + std::to_string(decodingtype));
    consolelog("ProcessManager",LogType::info,"binaural quality:\t " + std::to_string(binauralquality));
    consolelog("ProcessManager",LogType::info,"HRTF model type:\t " + std::to_string(hrtfmodel));
    char *error = sac_decode(input_char, output_char, bitstream_char, (double)this->fs, upmixtype, decodingtype, binauralquality, hrtfmodel);
    if(error == NULL) {
        consolelog("ProcessManager",LogType::progress,"decoding was completed successfully");
    } else {
        consolelog("ProcessManager",LogType::error,std::string(error));
    }
    return (error == NULL);
}

/**
 * @brief   It applys the selected effect to the input stream
 * @param   channels            boolean vector where true means to apply effect to that channel
 * @param   effect              effect object (it includes all parameters)
 * @return  true if it was successful
 */
bool ProcessManager::applyEffect(std::vector<bool> channels, Effect effect) {
    for (int channel = 0; channel < channels.size(); channel++) {
        if (channels[channel]) {

        } else {
            this->output[channel] = this->input[channel];
        }
    }
}
