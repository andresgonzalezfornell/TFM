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
    int **delay = (int **)std::malloc(2*sizeof(int *));
    if (method == "JCRev") {
        delay[0] = (int *)std::malloc(4*sizeof(int));
        delay[0][0] = std::round(0.108821*this->fs);
        delay[0][1] = std::round(0.113356*this->fs);
        delay[0][2] = std::round(0.122426*this->fs);
        delay[0][3] = std::round(0.133764*this->fs);
        delay[1] = (int *)std::malloc(3*sizeof(int));
        delay[1][0] = std::round(0.023832*this->fs);
        delay[1][1] = std::round(0.007642*this->fs);
        delay[1][2] = std::round(0.002562*this->fs);
    } else if (method == "freeverb") {
        delay[0] = (int *)std::malloc(8*sizeof(int));
        delay[0][0] = std::round(0.035306*this->fs);
        delay[0][1] = std::round(0.036667*this->fs);
        delay[0][2] = std::round(0.033810*this->fs);
        delay[0][3] = std::round(0.032245*this->fs);
        delay[0][4] = std::round(0.028957*this->fs);
        delay[0][5] = std::round(0.030748*this->fs);
        delay[0][6] = std::round(0.026939*this->fs);
        delay[0][7] = std::round(0.025306*this->fs);
        delay[1] = (int *)std::malloc(4*sizeof(int));
        delay[1][0] = std::round(0.005102*this->fs);
        delay[1][1] = std::round(0.012608*this->fs);
        delay[1][2] = std::round(0.010000*this->fs);
        delay[1][3] = std::round(0.007732*this->fs);
    }
    for (channel = 0; channel < (int)channels.size(); channel++) {
        filterindex = 0;
        if (method == "JCRev") {
            feedforwardfilter(input[channel], output[channel], samples, false, 0.25, 0.742, delay[0][0]);
            feedforwardfilter(input[channel], output[channel], samples, true, 0.25, 0.733, delay[0][1]);
            feedforwardfilter(input[channel], output[channel], samples, true, 0.25, 0.715, delay[0][2]);
            feedforwardfilter(input[channel], output[channel], samples, true, 0.25, 0.697, delay[0][3]);
            schroederfilter(input[channel], output[channel], samples, false, 1, phase, delay[1][0]);
            schroederfilter(output[channel], output[channel], samples, false, 1, phase, delay[1][1]);
            schroederfilter(output[channel], output[channel], samples, false, 1, phase, delay[1][2]);
        } else if (method == "freeverb") {
            lowpassfeedbackfilter(input[channel], output[channel], samples, false, 0.125, roomsize, damping, delay[0][0]);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, delay[0][1]);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, delay[0][2]);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, delay[0][3]);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, delay[0][4]);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, delay[0][5]);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, delay[0][6]);
            lowpassfeedbackfilter(input[channel], output[channel], samples, true, 0.125, roomsize, damping, delay[0][7]);
            schroederdiffusionfilter(output[channel], output[channel], samples, false, 1, 0.5, delay[1][0]);
            schroederdiffusionfilter(output[channel], output[channel], samples, false, 1, 0.5, delay[1][1]);
            schroederdiffusionfilter(output[channel], output[channel], samples, false, 1, 0.5, delay[1][2]);
            schroederdiffusionfilter(output[channel], output[channel], samples, false, 1, 0.5, delay[1][3]);
        } else {
            consolelog("Reverb", LogType::error, "unkown method \"" + method + "\"");
        }
    }
    std::free(delay[0]);
    std::free(delay[1]);
    std::free(delay);
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
