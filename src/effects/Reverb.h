#ifndef REVERB_H
#define REVERB_H

// Effect base header
#include "EffectBase.h"

/**
 * @class	Reverb
 * @author	Andrés González Fornell
 * @brief	Audio reverb effect.
 */
class Reverb AS_EFFECT {
public:
    Reverb();
    void apply(float *input, float *output, int samples);
    std::vector<std::vector<double>> plot(std::string chart);
    void schroederfilter(float *input, float *output, int samples, bool addition, float gain, int delay);
    void schroederdiffusionfilter(float *input, float *output, int samples, bool addition, float gain, int delay);
    void feedforwardfilter(float *input, float *output, int samples, bool addition, float original, int delay);
    void lowpassfeedforwardfilter(float *input, float *output, int samples, bool addition, float f, float d, int delay);
    void combfilter(float *input, float *output, int samples, bool addition, float *a, float *b, int order, float a_delay, float b_delay, int delay);
};

#endif // REVERB_H
