#include "Reverb.h"

/**
 * @brief   Reverb constructor.
 * @param   params          string of effect parameters
 */
Reverb::Reverb() AS_EFFECT_CONSTRUCTOR {
}

/**
 * @brief	It applies reverb effect.
 * @param 	input			input signal pointer
 * @param 	output			output signal pointer
 * @param 	samples			number of samples
 */
void Reverb::apply(float *input, float *output, int samples) {
    double dry = getDouble(params["dry"]);
    double roomsize = getDouble(params["roomsize"]);
    double damping = getDouble(params["damping"]);
    std::string method = params["method"];
    if (method == "JCRev") {
        feedforwardfilter(input, output, samples, false, 0.742, 4799);
        feedforwardfilter(input, output, samples, true, 0.733, 4999);
        feedforwardfilter(input, output, samples, true, 0.715, 5399);
        feedforwardfilter(input, output, samples, true, 0.697, 5899);
        schroederfilter(output, output, samples, false, dry, 1051);
        schroederfilter(output, output, samples, false, dry, 337);
        schroederfilter(output, output, samples, false, dry, 113);
    } else if (method == "freeverb") {
        lowpassfeedforwardfilter(input, output, samples, false, roomsize, damping, 1557);
        lowpassfeedforwardfilter(input, output, samples, true, roomsize, damping, 1617);
        lowpassfeedforwardfilter(input, output, samples, true, roomsize, damping, 1491);
        lowpassfeedforwardfilter(input, output, samples, true, roomsize, damping, 1422);
        lowpassfeedforwardfilter(input, output, samples, true, roomsize, damping, 1277);
        lowpassfeedforwardfilter(input, output, samples, true, roomsize, damping, 1356);
        lowpassfeedforwardfilter(input, output, samples, true, roomsize, damping, 1188);
        lowpassfeedforwardfilter(input, output, samples, true, roomsize, damping, 1116);
        schroederdiffusionfilter(output, output, samples, false, dry, 225);
        schroederdiffusionfilter(output, output, samples, false, dry, 556);
        schroederdiffusionfilter(output, output, samples, false, dry, 441);
        schroederdiffusionfilter(output, output, samples, false, dry, 341);
    } else {
        consolelog("Reverb", LogType::error, "unkown method \"" + method + "\"");
    }
}

/**
 * @brief   It sends some values to user interface charts.
 * @param   chart           chart id
 * @return  array of values as values[axis][sample]     axis: 0 = x (horizontal) and 1 = y (vertical)
 */
std::vector<std::vector<double>> Reverb::plot(std::string chart) {
    // Initialization
    const int N = 1000;
    std::vector<std::vector<double>> values = std::vector<std::vector<double>>(2, std::vector<double>(N));
    // Points
    for (int n = 0; n <= N; n++) {
        if (chart == "") {

        }
    }
    return values;
}

/**
 * @brief   It applies a shcroeder all pass filter, according to the transfer function H(z) = (g + z^-delay) / (1 + g·z^-delay).
 * @param 	input           input signal pointer
 * @param 	output          output signal pointer
 * @param 	samples			number of samples
 * @param   addition        true if the filter is in serie (filter output is summed to the existing output samples) and false if the filter is in cascade (filter output overwrites the existing output samples)
 * @param   gain            all pass filter gain
 * @param   delay           number of samples to delay
 */
void Reverb::schroederfilter(float *input, float *output, int samples, bool addition, float gain, int delay) {
    const int order = 0;
    // Initialization
    float *a = (float *)std::malloc((order+1)*sizeof(float));
    float *b = (float *)std::malloc((order+1)*sizeof(float));
    // Coefficients
    a[0] = 1;
    b[0] = gain;
    float a_delay = gain;
    float b_delay = 1;
    // Filtering
    combfilter(input, output, samples, addition, a, b, order, a_delay, b_delay, delay);
    // Cleaning
    std::free(a);
    std::free(b);
}

/**
 * @brief   It applies a schroeder diffusion filter, according to the transfer function H(z) = (-g + z^-delay) / (1 - g·z^-delay).
 * @param 	input           input signal pointer
 * @param 	output          output signal pointer
 * @param 	samples			number of samples
 * @param   addition        true if the filter is in serie (filter output is summed to the existing output samples) and false if the filter is in cascade (filter output overwrites the existing output samples)
 * @param   gain            all pass filter gain
 * @param   delay           number of samples to delay
 */
void Reverb::schroederdiffusionfilter(float *input, float *output, int samples, bool addition, float gain, int delay) {
    const int order = 0;
    // Initialization
    float *a = (float *)std::malloc((order+1)*sizeof(float));
    float *b = (float *)std::malloc((order+1)*sizeof(float));
    // Coefficients
    a[0] = 1;
    b[0] = -gain;
    float a_delay = -gain;
    float b_delay = 1;
    // Filtering
    combfilter(input, output, samples, addition, a, b, order, a_delay, b_delay, delay);
    // Cleaning
    std::free(a);
    std::free(b);
}

/**
 * @brief   It applies a feed forward comb filter, according to the transfer function H(z) = g + z^-delay.
 * @param 	input           input signal pointer
 * @param 	output          output signal pointer
 * @param 	samples			number of samples
 * @param   addition        true if the filter is in serie (filter output is summed to the existing output samples) and false if the filter is in cascade (filter output overwrites the existing output samples)
 * @param   original        gain of the original signal
 * @param   delay           number of samples to delay
 */
void Reverb::feedforwardfilter(float *input, float *output, int samples, bool addition, float original, int delay) {
    const int order = 0;
    // Initialization
    float *a = (float *)std::malloc((order+1)*sizeof(float));
    float *b = (float *)std::malloc((order+1)*sizeof(float));
    // Coefficients
    a[0] = 1;
    b[0] = original;
    float a_delay = 0;
    float b_delay = 1;
    // Filtering
    combfilter(input, output, samples, addition, a, b, order, a_delay, b_delay, delay);
    // Cleaning
    std::free(a);
    std::free(b);
}

/**
 * @brief   It applies a low pass feed forward comb filter, according to the transfer function H(z) = 1 / (1 - f · (1-d) / (1-d·z^-1) · z*-N).
 * @param 	input           input signal pointer
 * @param 	output          output signal pointer
 * @param 	samples			number of samples
 * @param   addition        true if the filter is in serie (filter output is summed to the existing output samples) and false if the filter is in cascade (filter output overwrites the existing output samples)
 * @param   f               feed forward comb filter gain
 * @param   d               low pass filter gain
 * @param   delay           number of samples to delay
 */
void Reverb::lowpassfeedforwardfilter(float *input, float *output, int samples, bool addition, float f, float d, int delay) {
    const int order = 1;
    // Initialization
    float *a = (float *)std::malloc((order+1)*sizeof(float));
    float *b = (float *)std::malloc((order+1)*sizeof(float));
    // Coefficients
    a[0] = 1;
    a[1] = 0;
    b[0] = 1;
    b[1] = -d;
    float a_delay = -d;
    float b_delay = -f*(1-d);
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
        int buffersize;
        if (delay > order) {
            buffersize = delay + 1;
        } else {
            buffersize = order + 1;
        }
        float *x = (float *)std::malloc(buffersize * sizeof(float)); // cyclic memory;
        float *y = (float *)std::malloc(buffersize * sizeof(float)); // cyclic memory;
        int pointer = 0; // pointer to the last element in memory
        for (int index = 0; index < buffersize; index++) {
            x[index] = 0;
            y[index] = 0;
        }
        for (int n = 0; n < samples; n++) {
            x[pointer] = input[n];
            // Filter
            y[pointer] = b[0] * x[pointer];
            for (int index = 1; index < order + 1; index++) {
                y[pointer] += b[index] * x[(buffersize + pointer - delay) % buffersize] - a[index] * y[(buffersize + pointer - delay) % buffersize];
            }
            y[pointer] += b_delay * x[(buffersize + pointer - delay) % buffersize] - a_delay * y[(buffersize + pointer - delay) % buffersize];
            y[pointer] /= a[0];
            if (addition) {
                output[n] += y[pointer];
            } else {
                output[n] = y[pointer];
            }
            // Memories update
            pointer = (pointer + 1) % buffersize;
        }
        // Cleaning
        std::free(x);
        std::free(y);
    }
}
