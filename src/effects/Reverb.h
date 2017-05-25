#ifndef REVERB_H
#define REVERB_H

// Effect base header
#include "EffectBase.h"

const int maxdelay = 5899;
const int maxfilters = 12;

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
    void schroederfilter(float *input, float *output, int samples, bool addition, float gain, float g, int delay);
    void schroederdiffusionfilter(float *input, float *output, int samples, bool addition, float gain, float g, int delay);
    void feedforwardfilter(float *input, float *output, int samples, bool addition, float gain, float original, int delay);
    void lowpassfeedbackfilter(float *input, float *output, int samples, bool addition, float gain, float rs, float d, int delay);
    void combfilter(float *input, float *output, int samples, bool addition, float *a, float *b, int order, float a_delay, float b_delay, int delay);
private:
    float x[maxfilters][maxdelay+1]; /**< filter buffer of input samples */
    float y[maxfilters][maxdelay+1]; /**< filter buffer of output samples */
    int pointer[maxfilters]; /**< pointer to the last element in memory */
    int filterindex; /**< filter counter in each iteration */
};

#endif // REVERB_H
