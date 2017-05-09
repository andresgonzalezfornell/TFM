#ifndef EQUALIZER_H
#define EQUALIZER_H

// Effect base header
#include "EffectBase.h"

/**
 * @class	Equalizer
 * @author	Andrés González Fornell
 * @brief	Audio compressor.
 */
class Equalizer AS_EFFECT {
public:
    Equalizer(std::map<std::string, std::string> params);
	void apply(float *input, float *output, int samples);
};

#endif // EQUALIZER_H
