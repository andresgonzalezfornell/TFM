#include "Panning.h"

/**
 * @brief   Panning constructor.
 */
Panning::Panning() AS_EFFECT_CONSTRUCTOR {
}

/**
 * @brief	It applies panning effect.
 * @param 	input			input signal pointer
 * @param 	output			output signal pointer
 * @param 	samples			number of samples
 * @param   channels        vector of channel types
 */
void Panning::apply(float **input, float **output, int samples, std::vector<SACBitstream::ChannelType::channeltype> channels) {
    float P;
    std::string ref_input, ref_output;
    for(int channel_output = 0; channel_output < (int)channels.size(); channel_output++) {
        ref_output = getChannelReference(channels[channel_output]);
        for(int channel_input = 0; channel_input < (int)channels.size(); channel_input++) {
            ref_input = getChannelReference(channels[channel_input]);
            P = getDouble(params[ref_output + "2" + ref_input]);
            for (int sample = 0; sample < samples; sample++) {
                if (channel_input == 0) {
                    output[channel_output][sample] = 0;
                }
                output[channel_output][sample] += input[channel_input][sample] * P;
            }
        }
    }
}

/**
 * @brief   It gets channel reference from a specified channel type
 * @param   channeltype     channel type
 * @return  string of channel type reference
 */
std::string Panning::getChannelReference(SACBitstream::ChannelType::channeltype channeltype) {
    switch (channeltype) {
    case SACBitstream::ChannelType::L:
        return "L";
        break;
    case SACBitstream::ChannelType::Lc:
        return "Lc";
        break;
    case SACBitstream::ChannelType::Ls:
        return "Ls";
        break;
    case SACBitstream::ChannelType::Lsr:
        return "Lsr";
        break;
    case SACBitstream::ChannelType::R:
        return "R";
        break;
    case SACBitstream::ChannelType::Rc:
        return "Rc";
        break;
    case SACBitstream::ChannelType::Rs:
        return "Rs";
        break;
    case SACBitstream::ChannelType::Rsr:
        return "Rsr";
        break;
    case SACBitstream::ChannelType::C:
        return "C";
        break;
    case SACBitstream::ChannelType::LFE:
        return "LFE";
        break;
    default:
        return NULL;
        break;
    }
}
