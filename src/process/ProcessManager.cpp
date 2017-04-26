// Class libraries
#include "ProcessManager.h"

/**
 * @brief   ProcessManager constructor.
 * @param   fs                  signal sampling frequency
 * @param   chunksize           number of samples in a chunk to apply effect step by step (if 0 then chunk size is the number of samples and effect is applied at once)
 */
ProcessManager::ProcessManager(int fs, int chunksize) {
    this->fs = fs;
    this->chunksize = chunksize;
    this->clear();
    consolelog("ProcessManager",LogType::progress,"ProgressManager object is created");
}

/**
 * @brief   ProcessManager destructor.
 */
ProcessManager::~ProcessManager() {
    this->clear();
    consolelog("ProcessManager",LogType::progress,"ProgressManager object is deleted");
}

/**
 * @brief   It sets input variable from the existing input file.
 * @param   filename            audio input file name
 */
void ProcessManager::setInput(std::string filename) {
    this->clear();
    this->inputfile = new WAVFile(filename, false);
    if((int)this->inputfile->header.samplerate != this->fs) {
        this->clear();
        consolelog("ProcessManager",LogType::error, "input file sampling frequency is not valid (it is " + std::to_string(this->inputfile->header.samplerate) + "Hz and it should be " + std::to_string(this->fs) + "Hz)");
    } else {
        this->inputfile->setCursor(0);
        this->channels = this->inputfile->header.numchannels;
        this->samples = this->inputfile->samples() / this->inputfile->header.numchannels;
        // Variables creation
        this->input = new float*[this->channels];
        this->output = new float*[this->channels];
        for (int channel = 0; channel < this->channels; channel++) {
            this->input[channel] = new float[this->samples];
            this->output[channel] = new float[this->samples];
        }
        // Input reading
        for (int sample = 0; sample < this->samples; sample++) {
            for (int channel = 0; channel < this->channels; channel++) {
                this->input[channel][sample] = this->inputfile->readValue();
            }
        }
        consolelog("ProcessManager",LogType::progress, "input file has been loaded");
    }
}

/**
 * @brief   It sets an output file from the existing output variable
 * @param   filename            audio output file name
 */
void ProcessManager::setOutput(std::string filename) {
    this->outputfile = new WAVFile(filename,this->channels,this->fs,this->inputfile->header.bitspersample);
    for (int sample = 0; sample < this->samples; sample++) {
        for (int channel = 0; channel < this->channels; channel++) {
            this->outputfile->writeValue(this->output[channel][sample]);
        }
    }
    this->outputfile->writeHeader(); // Updating output file header
    consolelog("ProcessManager",LogType::progress, "output file has been created");
}

/**
 * @brief   It performs the SAC encoder.
 * @param   input               filename of the multichannel input audio file
 * @param   outpu               filename of the downmix output audio file (it will be automatically created)
 * @param   bitstream           filename of the bitstream output file or "buried" (it will be automatically created)
 * @param   upmixtype           upmix type              0: normal       1: blind        2: binaural     3: stereo
 * @param   decodingtype        decoding type           0: low          1: high
 * @param   binauralquality     binaural upmix quality  0: parametric   1: filtering
 * @param   hrtfmodel           HRTF model              0: kemar        1: vast         2: mps_vt
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
    this->setInput(output);
    return (error == NULL);
}

/**
 * @brief   It applys the selected effect to the input stream
 * @param   channels            boolean vector where true means to apply effect to that channel
 * @param   effect              effect object (it includes all parameters)
 * @return  true if it was successful
 */
bool ProcessManager::applyEffect(std::vector<bool> channels, Effect effect) {   
    if ((int)channels.size() == this->channels) {
        int n, N;
        if (this->chunksize == 0) {
            n = 0;
            N = this->samples;
        } else {
            n = this->realtime;
            N = this->chunksize;
        }
        for (int channel = 0; channel < this->channels; channel++) {
            if (channels[channel]) {
                effect.apply(input[channel]+n, output[channel]+n, N);
            } else {
                for(int sample = n; sample < (n+N); sample++) {
                    this->output[channel][sample] = this->input[channel][sample];
                }
            }
        }
        this->realtime += this->chunksize;
        return false;
    } else {
        consolelog("ProcessManager",LogType::error,"channels boolean vector (from effect argument) = " + std::to_string(channels.size()) + " does not correspond to number of channels = " + std::to_string(this->channels));
        return false;
    }
}

/**
 * @brief   It clears all variables and resets the process.
 */
void ProcessManager::clear() {
    this->channels = 0;
    this->realtime = 0;
    this->inputfile = NULL;
    this->outputfile = NULL;
}
