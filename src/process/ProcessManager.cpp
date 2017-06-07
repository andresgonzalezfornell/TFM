// Class libraries
#include "ProcessManager.h"

/**
 * @brief   ProcessManager constructor.
 * @param   chunksize           number of samples in a chunk to apply effect step by step (if 0 then chunk size is the number of samples and effect is applied at once)
 */
ProcessManager::ProcessManager(int chunksize) {
    this->allocated = false;
    this->chunksize = chunksize;
    this->clear();
    consolelog("ProcessManager", LogType::progress,
               "ProgressManager object is created");
}

/**
 * @brief   ProcessManager destructor.
 */
ProcessManager::~ProcessManager() {
    this->clear();
    consolelog("ProcessManager", LogType::progress,
               "ProgressManager object is deleted");
}

/**
 * @brief   It sets input variable from the existing input file.
 * @param   filename            audio input file name
 * @return  true if it was successful
 */
bool ProcessManager::setInput(std::string filename) {
    this->inputfile = new WAVFile(filename, false);
    this->channels = this->inputfile->header.numchannels;
    this->samples = this->inputfile->samples() / this->inputfile->header.numchannels;
    if (this->inputfile->header.samplerate != (unsigned int)this->fs) {
        consolelog("ProcessManager", LogType::error, "frequency sampling of decoded file (" + std::to_string(this->inputfile->header.samplerate) + " Hz) does not match specified frequency sampling (" + std::to_string(this->fs) + " Hz)");
        return false;
    }
    // Input reading
    this->inputfile->setCursor(0);
    this->input = this->inputfile->readSamples(this->samples);
    // Output initialization
    this->output = (float **)std::malloc(this->channels * sizeof(float *));
    for (int channel = 0; channel < this->channels; channel++) {
        this->output[channel] = (float *)std::malloc(this->samples * sizeof(float));
    }
    this->allocated = true;
    consolelog("ProcessManager", LogType::progress, "input file has been loaded");
    return true;
}

/**
 * @brief   It sets an output file from the existing output variable
 * @param   filename            audio output file name
 * @return  true if it was successful
 */
bool ProcessManager::setOutput(std::string filename) {
    this->outputfile = new WAVFile(filename, this->channels, this->fs,
                                   this->inputfile->header.bitspersample);
    this->outputfile->writeSamples(this->output, this->samples);
    consolelog("ProcessManager", LogType::progress,
               "output file has been created");
    return true;
}

/**
 * @brief   It performs the SAC encoder.
 * @param   input               filename of the multichannel input audio file
 * @param   output              filename of the downmix output audio file (it will be automatically created)
 * @param   bitstream           filename of the bitstream output file or "buried" (it will be automatically created)
 * @param   upmixtype           upmix type              0: normal       1: blind        2: binaural     3: stereo
 * @param   decodingtype        decoding type           0: low          1: high
 * @param   binauralquality     binaural upmix quality  0: parametric   1: filtering
 * @param   hrtfmodel           HRTF model              0: kemar        1: vast         2: mps_vt
 * @return  true if it was successful
 */
bool ProcessManager::decode(std::string input, std::string bitstream,
                            std::string output, int decodingtype, int upmixtype,
                            int binauralquality, int hrtfmodel) {
    this->clear();
    const char *input_char = input.c_str();
    const char *bitstream_char = bitstream.c_str();
    const char *output_char = output.c_str();
    if (bitstream == "buried") {
        this->bitstream = new SACBitstream();
    } else {
        this->bitstream = new SACBitstream(bitstream);
    }
    this->fs = this->bitstream->fs;
    consolelog("ProcessManager", LogType::info, "input file:\t\t " + input);
    consolelog("ProcessManager", LogType::info,
               "bitstream file:\t " + bitstream);
    consolelog("ProcessManager", LogType::info, "output file:\t\t " + output);
    consolelog("ProcessManager", LogType::info,
               "upmix type:\t " + std::to_string(upmixtype));
    consolelog("ProcessManager", LogType::info,
               "sampling frequency:\t " + std::to_string(this->fs) + "Hz");
    consolelog("ProcessManager", LogType::info,
               "upmix type:\t\t " + std::to_string(upmixtype));
    consolelog("ProcessManager", LogType::info,
               "decoding type:\t " + std::to_string(decodingtype));
    consolelog("ProcessManager", LogType::info,
               "binaural quality:\t " + std::to_string(binauralquality));
    consolelog("ProcessManager", LogType::info,
               "HRTF model type:\t " + std::to_string(hrtfmodel));
    char *error = sac_decode(input_char, output_char, bitstream_char,
                             (double) this->fs, upmixtype, decodingtype, binauralquality,
                             hrtfmodel);
    if (error != NULL) {
        consolelog("ProcessManager", LogType::error, std::string(error));
        return false;
    } else {
        consolelog("ProcessManager", LogType::progress,
                   "decoding was completed successfully");
        // Input file
        if (!this->setInput(output)) {
            consolelog("ProcessManager", LogType::error, "error while writing on decoded input file");
            return false;
        } else {
            return true;
        }
    }
}

/**
 * @brief   It applys the selected effect to the input stream
 * @param   effect              effect object (it includes all parameters)
 * @param   channels            boolean vector where true means to apply effect to that channel
 * @param   levels              vector of input levels (>=0) for each channel
 * @return  true if it was successful
 */
bool ProcessManager::applyEffect(Effect *effect, std::vector<bool> channels,
                                 std::vector<double> levels) {
    if (this->cursor < this->samples) {
        if ((int) channels.size() == this->channels) {
            int n, N;
            if (this->chunksize == 0) {
                n = 0;
                N = this->samples;
            } else {
                n = this->cursor;
                N = this->chunksize;
                if (n + N >= this->samples) {
                    N = this->samples - n;
                }
            }
            float **input_effect = (float **) std::malloc(this->channels * sizeof(float *));
            float **output_effect = (float **) std::malloc(this->channels * sizeof(float *));
            for (int channel = 0; channel < this->channels; channel++) {
                input_effect[channel] = (float *) std::malloc(N * sizeof(float));
                output_effect[channel] = (float *) std::malloc(N * sizeof(float));
                // Pre-amplifier
                switch (this->bitstream->channels[channel]) {
                case SACBitstream::ChannelType::Ls:
                case SACBitstream::ChannelType::Lsr:
                case SACBitstream::ChannelType::Rs:
                case SACBitstream::ChannelType::Rsr:
                    levels[channel] *= this->bitstream->gain_surround;
                    break;
                case SACBitstream::ChannelType::LFE:
                    levels[channel] *= this->bitstream->gain_LFE;
                    break;
                default:
                    break;
                }
                // Levels
                for (int sample = n; sample < (n + N); sample++) {
                    input_effect[channel][sample - n] = levels[channel] * this->input[channel][sample];
                }
            }
            // Effect
            effect->apply(input_effect, output_effect, N, this->bitstream->channels);
            for (int channel = 0; channel < this->channels; channel++) {
                // Channels selection
                if (channels[channel]) {
                    std::memcpy(this->output[channel] + n, output_effect[channel], N*sizeof(float));
                } else {
                    std::memcpy(this->output[channel] + n, input_effect[channel], N*sizeof(float));
                }
                // Post-amplifier
                switch (this->bitstream->channels[channel]) {
                case SACBitstream::ChannelType::Ls:
                case SACBitstream::ChannelType::Lsr:
                case SACBitstream::ChannelType::Rs:
                case SACBitstream::ChannelType::Rsr:
                    levels[channel] /= this->bitstream->gain_surround;
                    for (int sample = n; sample < (n + N); sample++) {
                        this->output[channel][sample] /= this->bitstream->gain_surround;
                    }
                    break;
                case SACBitstream::ChannelType::LFE:
                    levels[channel] /= this->bitstream->gain_LFE;
                    for (int sample = n; sample < (n + N); sample++) {
                        this->output[channel][sample] /= this->bitstream->gain_LFE;
                    }
                    break;
                default:
                    break;
                }
            }
            std::free(input_effect);
            std::free(output_effect);
            this->cursor += N;
            if (this->cursor > this->total) {
                this->total = this->cursor;
            }
            return true;
        } else {
            consolelog("ProcessManager", LogType::error,
                       "channels boolean vector (from effect argument) = "
                       + std::to_string(channels.size())
                       + " does not correspond to number of channels = "
                       + std::to_string(this->channels));
            return false;
        }
    } else {
        consolelog("ProcessManager", LogType::error, "there is no more available samples (samples = " + std::to_string(this->samples) + ")");
        return false;
    }
}

/**
 * @brief   It clears all variables and resets the process.
 */
void ProcessManager::clear() {
    this->channels = 0;
    this->samples = 0;
    this->cursor = 0;
    this->total = 0;
    if (this->allocated) {
        std::free(this->input);
        std::free(this->output);
        this->allocated = false;
    }
    this->inputfile = NULL;
    this->outputfile = NULL;
}
