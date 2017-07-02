#include "Reverb.h"

/**
 * @brief   Reverb constructor.
 */
Reverb::Reverb() AS_EFFECT_CONSTRUCTOR {
    for (int i = 0; i < maxchannels; i++) {
        for (int j = 0; j < maxfilters; j++) {
            pointer[i][j] = 0;
        }
    }
}

/**
 * @brief	It applies reverb effect.
 * @param 	input			input signal pointer
 * @param 	output			output signal pointer
 * @param 	samples			number of samples
 * @param   channels        vector of channel types
 */
void Reverb::apply(float **input, float **output, int samples, std::vector<SACBitstream::ChannelType::channeltype> channels) {
    double phase = getDouble(params["phase"]);
    double roomsize = getDouble(params["roomsize"]);
    double damping = getDouble(params["damping"]);
    std::string method = params["method"];
    for (channel = 0; channel < (int)channels.size(); channel++) {
        filterindex = 0;
        if (method == "JCRev") {
            feedforwardfilter(input[channel], output[channel], samples, false, 0.25, 0.742, 4799);
            feedforwardfilter(input[channel], output[channel], samples, true, 0.25, 0.733, 4999);
            feedforwardfilter(input[channel], output[channel], samples, true, 0.25, 0.715, 5399);
            feedforwardfilter(input[channel], output[channel], samples, true, 0.25, 0.697, 5899);
            schroederfilter(input[channel], output[channel], samples, false, 1, phase, 1051);
            schroederfilter(output[channel], output[channel], samples, false, 1, phase, 337);
            schroederfilter(output[channel], output[channel], samples, false, 1, phase, 113);
        } else if (method == "freeverb") {
            lowpassfeedbackfilter(input[channel], output[channel], samples, false, 0.125, roomsize, damping, 1557);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, 1617);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, 1491);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, 1422);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, 1277);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, 1356);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, 1188);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, 1116);
            schroederdiffusionfilter(output[channel], output[channel], samples, false, 1, 0.5, 225);
            schroederdiffusionfilter(output[channel], output[channel], samples, false, 1, 0.5, 556);
            schroederdiffusionfilter(output[channel], output[channel], samples, false, 1, 0.5, 441);
            schroederdiffusionfilter(output[channel], output[channel], samples, false, 1, 0.5, 341);
        } else {
            consolelog("Reverb", LogType::error, "unkown method \"" + method + "\"");
        }
    }
}

/**
 * @brief   It applies a shcroeder allpass filter, according to the transfer function H(z) = gain * (g + z^-delay) / (1 + g·z^-delay).
 * @param 	input           input signal pointer
 * @param 	output          output signal pointer
 * @param 	samples			number of samples
 * @param   addition        true if the filter is in serie (filter output is summed to the existing output samples) and false if the filter is in cascade (filter output overwrites the existing output samples)
 * @param   gain            function transfer gain
 * @param   g               all pass filter gain
 * @param   delay           number of samples to delay
 */
void Reverb::schroederfilter(float *input, float *output, int samples, bool addition, float gain, float g, int delay) {
    const int order = 0;
    // Initialization
    float *a = (float *)std::malloc((order+1)*sizeof(float));
    float *b = (float *)std::malloc((order+1)*sizeof(float));
    // Coefficients
    a[0] = 1;
    b[0] = gain * g;
    float a_delay = g;
    float b_delay = gain;
    // Filtering
    combfilter(input, output, samples, addition, a, b, order, a_delay, b_delay, delay);
    // Cleaning
    std::free(a);
    std::free(b);
}

/**
 * @brief   It applies a schroeder diffusion allpass filter, according to the transfer function H(z) = gain * (-g + z^-delay) / (1 - g·z^-delay).
 * @param 	input           input signal pointer
 * @param 	output          output signal pointer
 * @param 	samples			number of samples
 * @param   addition        true if the filter is in serie (filter output is summed to the existing output samples) and false if the filter is in cascade (filter output overwrites the existing output samples)
 * @param   gain            function transfer gain
 * @param   g               all pass filter gain
 * @param   delay           number of samples to delay
 */
void Reverb::schroederdiffusionfilter(float *input, float *output, int samples, bool addition, float gain, float g, int delay) {
    const int order = 0;
    // Initialization
    float *a = (float *)std::malloc((order+1)*sizeof(float));
    float *b = (float *)std::malloc((order+1)*sizeof(float));
    // Coefficients
    a[0] = 1;
    b[0] = gain * (-g);
    float a_delay = -g;
    float b_delay = gain;
    // Filtering
    combfilter(input, output, samples, addition, a, b, order, a_delay, b_delay, delay);
    // Cleaning
    std::free(a);
    std::free(b);
}

/**
 * @brief   It applies a feed forward comb filter, according to the transfer function H(z) = gain * (g + z^-delay).
 * @param 	input           input signal pointer
 * @param 	output          output signal pointer
 * @param 	samples			number of samples
 * @param   addition        true if the filter is in serie (filter output is summed to the existing output samples) and false if the filter is in cascade (filter output overwrites the existing output samples)
 * @param   gain            function transfer gain
 * @param   original        gain of the original signal
 * @param   delay           number of samples to delay
 */
void Reverb::feedforwardfilter(float *input, float *output, int samples, bool addition, float gain, float original, int delay) {
    const int order = 0;
    // Initialization
    float *a = (float *)std::malloc((order+1)*sizeof(float));
    float *b = (float *)std::malloc((order+1)*sizeof(float));
    // Coefficients
    a[0] = 1;
    b[0] = gain * original;
    float a_delay = 0;
    float b_delay = gain;
    // Filtering
    combfilter(input, output, samples, addition, a, b, order, a_delay, b_delay, delay);
    // Cleaning
    std::free(a);
    std::free(b);
}

/**
 * @brief   It applies a Schroeder-Moorer low pass feedback comb filter, according to the transfer function H(z) = gain / (1 - f · (1-d) / (1-d·z^-1) · z*-N).
 * @param 	input           input signal pointer
 * @param 	output          output signal pointer
 * @param 	samples			number of samples
 * @param   addition        true if the filter is in serie (filter output is summed to the existing output samples) and false if the filter is in cascade (filter output overwrites the existing output samples)
 * @param   gain            function transfer gain
 * @param   rs              feed forward comb filter gain
 * @param   d               low pass filter gain
 * @param   delay           number of samples to delay
 */
void Reverb::lowpassfeedbackfilter(float *input, float *output, int samples, bool addition, float gain, float rs, float d, int delay) {
    const int order = 1;
    // Initialization
    float *a = (float *)std::malloc((order+1)*sizeof(float));
    float *b = (float *)std::malloc((order+1)*sizeof(float));
    // Coefficients
    a[0] = 1;
    a[1] = -d;
    b[0] = gain;
    b[1] = gain * (-d);
    float a_delay = -rs*(1-d);
    float b_delay = 0;
    // Filtering
    combfilter(input, output, samples, addition, a, b, order, a_delay, b_delay, delay);
    // Cleaning
    std::free(a);
    std::free(b);
}

/**
 * @brief   It applies a comb filter, according to the transfer function H(z) = (b[0] + b[1]·z^-1 + ... + b[order]·z^-order + b_delay·z^-delay) / (a[0] + a[1]·z^-1 + ... + a[order]·z^-order + a_delay·z^-delay).
 * @param 	input           input signal pointer
 * @param 	output          output signal pointer
 * @param 	samples			number of samples
 * @param   addition        true if the filter is in serie (filter output is summed to the existing output samples) and false if the filter is in cascade (filter output overwrites the existing output samples)
 * @param   a               y coefficients of transfer function
 * @param   b               x coefficients of transfer function
 * @param   order           filter order (value of the highest exponent) without including delay term
 * @param   a_delay         y delay coefficients of transfer function
 * @param   b_delay         x delay coefficients of transfer function
 * @param   delay           number of samples to delay
 */
void Reverb::combfilter(float *input, float *output, int samples, bool addition, float *a, float *b, int order, float a_delay, float b_delay, int delay) {
    if (a[0] != 0) {
        // Normalization of coefficients
        for (int index = 0; index < order + 1; index++) {
            a[index] /= a[0];
            b[index] /= a[0];
        }
        int buffersize;
        if (delay > order) {
            buffersize = delay + 1;
            a_delay /= a[0];
            b_delay /= a[0];
        } else {
            buffersize = order + 1;
        }
        int n = pointer[channel][filterindex];
        for (int sample = 0; sample < samples; sample++) {
            x[channel][filterindex][n] = input[sample];
            // Filter
            y[channel][filterindex][n] = b[0] * x[channel][filterindex][n];
            for (int index = 1; index < order + 1; index++) {
                y[channel][filterindex][n] += b[index] * x[channel][filterindex][(buffersize + n - index) % buffersize] - a[index] * y[channel][filterindex][(buffersize + n - index) % buffersize];
            }
            y[channel][filterindex][n] += b_delay * x[channel][filterindex][(buffersize + n - delay) % buffersize] - a_delay * y[channel][filterindex][(buffersize + n - delay) % buffersize];
            if (addition) {
                output[sample] += y[channel][filterindex][n];
            } else {
                output[sample] = y[channel][filterindex][n];
            }
            // Memories update
            n = (n + 1) % buffersize;
        }
        pointer[channel][filterindex] = n;
    }
    filterindex++;
}
