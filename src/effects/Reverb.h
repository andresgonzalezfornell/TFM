#ifndef REVERB_H
#define REVERB_H

// Effect base header
#include "EffectBase.h"

/**
 * @class	Reverb
 * @author	Andrés González Fornell
 * @brief	Audio reverb effect.
 */
class Reverb /** @cond */ AS_EFFECT /** @endcond */ {
public:
    Reverb();
    void apply(float **input, float **output, int samples, std::vector<SACBitstream::ChannelType::channeltype> channels);
    void schroederfilter(float *input, float *output, int samples, bool addition, float gain, float g, int delay);
    void schroederdiffusionfilter(float *input, float *output, int samples, bool addition, float gain, float g, int delay);
    void feedforwardfilter(float *input, float *output, int samples, bool addition, float gain, float original, int delay);
    void lowpassfeedbackfilter(float *input, float *output, int samples, bool addition, float gain, float rs, float d, int delay);
    void combfilter(float *input, float *output, int samples, bool addition, float *a, float *b, int order, float a_delay, float b_delay, int delay);
private:
    static const int maxchannels = 8; /**< maximum number of channels */
    static const int maxfilters = 12; /**< maximum number of filters */
    static const int maxdelay = 5899; /**< maximum delay in samples */
    float x[maxchannels][maxfilters][maxdelay+1]; /**< filter buffer of input samples */
    float y[maxchannels][maxfilters][maxdelay+1]; /**< filter buffer of output samples */
    int pointer[maxchannels][maxfilters]; /**< pointer to the last element in memory */
    int filterindex; /**< filter counter in each iteration */
    int channel; /**< channel index */
};

#endif // REVERB_H
