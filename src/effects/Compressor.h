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
    Compressor(std::map<std::string, std::string> params);
    void apply(float *input, float *output, int samples);
};

#endif // COMPRESSOR_H
