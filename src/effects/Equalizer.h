#ifndef EQUALIZER_H
#define EQUALIZER_H

// Effect base header
#include "EffectBase.h"

/**
 * @class	Equalizer
 * @author	Andrés González Fornell
 * @brief	Audio equalizer effect.
 */
class Equalizer /** @cond */ AS_EFFECT /** @endcond */ {
public:
    Equalizer();
    void apply(float **input, float **output, int samples, std::vector<SACBitstream::ChannelType::channeltype> channels);
    void peakingFilter(float *input, float *output, int samples, double f_0, double gain, double Q, int order);
    void lowShelfFilter(float *input, float *output, int samples, double f_0, double gain, int order);
    void highShelfFilter(float *input, float *output, int samples, double f_0, double gain, int order);
    void filter(float *x, float *y, int samples, float *a, float *b, int order);
private:
    static const int maxchannels = 8; /**< maximum number of channels */
    static const int bands = 10; /**< number of equalizer bands */
    static const int order = 2; /**< filters order */
    double frequency[bands] = {31.25,
                               62.5,
                               125.0,
                               250.0,
                               500.0,
                               1000.0,
                               2000.0,
                               4000.0,
                               8000.0,
                               16000.0}; /**< center frequencies [Hz] */

    float x[maxchannels][bands][order+1]; /**< filter buffer of input samples */
    float y[maxchannels][bands][order+1]; /**< filter buffer of output samples */
    int filterindex; /**< filter counter in each iteration */
    int channel; /**< channel index */
};

#endif // EQUALIZER_H
