#include "Compressor.h"

/**
 * @brief   Compressor constructor.
 * @param   params          string of effect parameters
 */
Compressor::Compressor() AS_EFFECT_CONSTRUCTOR {
}

/**
 * @brief	It applies compression effect.
 * @param 	input			input signal pointer
 * @param 	output			output signal pointer
 * @param 	samples			number of samples
 */
void Compressor::apply(float *input, float *output, int samples) {
    this->update();
    for (int sample = 0; sample < samples; sample++) {
        float value = input[sample];
        output[sample] = value;
    }
}

/**
 * @brief   It sends some values to user interface charts.
 * @param   chart           chart id
 * @return  array of values as values[axis][sample]     axis: 0 = x (horizontal) and 1 = y (vertical)
 */
std::vector<std::vector<double>> Compressor::plot(std::string chart) {
    this->update();
    // Initialization
    const int N = 1000;
    std::vector<std::vector<double>> values = std::vector<std::vector<double>>(2, std::vector<double>(N));
    // Points
    for (int n = 0; n <= N; n++) {
        if (chart == "compressor") {
            values[0][n] = 50*((double)n/N - 1);
            values[1][n] = values[0][n] + this->gain(values[0][n]);
        }
    }
    return values;
}

/**
 * @brief   It sets params from map of params.
 */
void Compressor::update() {
    this->threshold = getDouble(params["threshold"]);
    this->ratio = getDouble(params["ratio"]);
    this->attack = getDouble(params["attack"]);
    this->release = getDouble(params["release"]);
    if (params["type"] == "upward") {
        this->type = Compressor::upward;
    } else {
        this->type = Compressor::downward;
    }
}

/**
 * @brief   It calculates the gain for a given input level.
 * @param   inputlevel      input level [dB]
 * @return  compressor gain [dB]
 */
double Compressor::gain(double inputlevel) {
    switch(this->type) {
    case Compressor::downward:
    default:
        if (inputlevel < threshold) {
            return 0;
        } else {
            return (1/ratio - 1) * (inputlevel - threshold);
        }
        break;
    case Compressor::upward:
        if (inputlevel < threshold) {
            return (1/ratio - 1) * (inputlevel - threshold);
        } else {
            return 0;
        }
        break;
    }
}
