#ifndef PANNING_H
#define PANNING_H

// Effect base header
#include "EffectBase.h"

/**
 * @class	Panning
 * @author	Andrés González Fornell
 * @brief	Audio panning effect.
 */
class Panning /** @cond */ AS_EFFECT /** @endcond */ {
public:
    Panning();
    void apply(float **input, float **output, int samples, std::vector<SACBitstream::ChannelType::channeltype> channels);
private:
    std::string getChannelReference(SACBitstream::ChannelType::channeltype channeltype);
};

#endif // PANNING_H
