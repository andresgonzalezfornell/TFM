#ifndef COMPRESSOR_H
#define COMPRESSOR_H

// Effect base header
#include "EffectBase.h"

/**
 * @class	Compressor
 * @author	Andrés González Fornell
 * @brief	Audio compressor.
 */
class Compressor AS_EFFECT {
public:
    Compressor();
    void apply(float *input, float *output, int samples, SACBitstream::ChannelType::channeltype channel);
    std::vector<std::vector<double> > plot(std::string chart);
    void update();
private:
    enum compressortype {
        downward, /**< downward compression (reduces loud sounds over a certain threshold while quiet sounds remain unaffected) */
        upward /**< upward compression (increases the loudness of sounds below a certain threshold while leaving louder sounds unaffected) */
    };
    enum compressorfunction {
        compression, /**< effect is a compressor */
        expansion /**< effect is a expander */
    };
    Compressor::compressortype type; /**< compressor type */
    Compressor::compressorfunction function; /**< compressor function */
    // Parameters
    double threshold; /**< input level threshold for compressing [dB] */
    double ratio; /**< compression ratio */
    double gain(double inputlevel);
};

#endif // COMPRESSOR_H
