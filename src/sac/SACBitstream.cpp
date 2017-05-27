#include "SACBitstream.h"

/**
 * @brief   Bitstream constructor.
 */
SACBitstream::SACBitstream(std::string filename) {
    this->bitstream = new File(filename, false);
    this->load();
    consolelog("Bitstream", LogType::progress, "bitstream object is created");
}

/**
 * @brief   Bitstream destructor.
 */
SACBitstream::~SACBitstream() {
    std::free(this->channel);
    consolelog("Bitstream", LogType::progress, "bitstream object is deleted");
}

/**
 * @brief   It gets the value of a bitstream variable.
 * @param   position            position in bits
 * @param   length              number of bits
 * @return  value of the bitstream variable
 */
long int SACBitstream::getVariable(int position, int length) {
    long int value = 0;
    int byte = position / 8;
    int bit = position % 8;
    int bytes = std::ceil((double)(bit + length) / 8);
    int bits = (bit + length) % 8;
    if (bits == 0) {
        bits = 8;
    }
    this->bitstream->setCursor(byte);
    std::string data = bitstream->readText(bytes);
    for (int index = 0; index < bytes; index++) {
        char mask = 0xFF;
        if (index == 0) {
            mask = pow(2, 8 - bit) - 1;
        }
        char databyte;
        if(index == bytes - 1) {
            databyte = (data[index] & mask) >> (8 - bits);
            value = value << bits;
        } else {
            databyte = data[index] & mask;
            value = value << 8;
        }
        value |= databyte;
    }
    return value;
}

/**
 * @brief   It loads variables from bitstream file.
 */
void SACBitstream::load() {
    int additional = 0;
    // Sampling frequency
    int fs_index = this->getVariable(8 + additional, 4);
    switch (fs_index) {
    case 0x0:
        this->fs = 96000;
        break;
    case 0x1:
        this->fs = 88200;
        break;
    case 0x2:
        this->fs = 64000;
        break;
    case 0x3:
        this->fs = 48000;
        break;
    default:
    case 0x4:
        this->fs = 44100;
        break;
    case 0x5:
        this->fs = 32000;
        break;
    case 0x6:
        this->fs = 24000;
        break;
    case 0x7:
        this->fs = 22050;
        break;
    case 0x8:
        this->fs = 16000;
        break;
    case 0x9:
        this->fs = 12000;
        break;
    case 0xA:
        this->fs = 11025;
        break;
    case 0xB:
        this->fs = 8000;
        break;
    case 0xC:
        this->fs = 7350;
        break;
    case 0xF:
        this->fs = this->getVariable(12, 24);
        additional += 24;
        break;
    }
    // Tree config
    int treeconfig = this->getVariable(22 + additional, 4);
    switch (treeconfig) {
    default:
    case 5151:
        this->channel = (ChannelType::channeltype *)std::malloc(6*sizeof(ChannelType::channeltype));
        this->channel[0] = ChannelType::L;
        this->channel[1] = ChannelType::R;
        this->channel[2] = ChannelType::C;
        this->channel[3] = ChannelType::LFE;
        this->channel[4] = ChannelType::Ls;
        this->channel[5] = ChannelType::Rs;
        break;
    case 5152:
    case 525:
        this->channel = (ChannelType::channeltype *)std::malloc(6*sizeof(ChannelType::channeltype));
        this->channel[0] = ChannelType::L;
        this->channel[1] = ChannelType::Ls;
        this->channel[2] = ChannelType::R;
        this->channel[3] = ChannelType::Rs;
        this->channel[4] = ChannelType::C;
        this->channel[5] = ChannelType::LFE;
        break;
    case 7271:
    case 7571:
        this->channel = (ChannelType::channeltype *)std::malloc(8*sizeof(ChannelType::channeltype));
        this->channel[0] = ChannelType::L;
        this->channel[1] = ChannelType::Lc;
        this->channel[2] = ChannelType::Ls;
        this->channel[3] = ChannelType::R;
        this->channel[4] = ChannelType::Rc;
        this->channel[5] = ChannelType::Rs;
        this->channel[6] = ChannelType::C;
        this->channel[7] = ChannelType::LFE;
        break;
    case 7272:
    case 7572:
        this->channel = (ChannelType::channeltype *)std::malloc(8*sizeof(ChannelType::channeltype));
        this->channel[0] = ChannelType::L;
        this->channel[1] = ChannelType::Lsr;
        this->channel[2] = ChannelType::Ls;
        this->channel[3] = ChannelType::R;
        this->channel[4] = ChannelType::Rsr;
        this->channel[5] = ChannelType::Rs;
        this->channel[6] = ChannelType::C;
        this->channel[7] = ChannelType::LFE;
        break;
    }
    // Gain of surround channels
    int gain_surround_index = this->getVariable(30 + additional, 3);
    if (gain_surround_index > 4) {
        gain_surround_index = 0;
    }
    this->gain_surround = std::pow(2, (double) gain_surround_index / 2);
    // Gain of LFE channels
    int gain_LFE_index = this->getVariable(33 + additional, 3);
    if (gain_LFE_index > 4) {
        gain_LFE_index = 0;
    }
    this->gain_LFE = std::pow(10, (double) gain_LFE_index / 2);
    // Gain of downmix
    int gain_downmix_index = this->getVariable(36 + additional, 3);
    if (gain_downmix_index > 4) {
        gain_downmix_index = 0;
    } else if(gain_downmix_index == 7) {
        gain_downmix_index = 8;
    }
    this->gain_downmix = std::pow(2, (double) gain_downmix_index / 4);
}
