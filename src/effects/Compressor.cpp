#include "Compressor.h"

/**
 * @brief   Compressor constructor.
 * @param   params          string of effect parameters
 */
Compressor::Compressor(std::string params) EFFECT_CONSTRUCTOR {
}

/**
 * @brief	It applies compression effect.
 * @param 	input			input signal pointer
 * @param 	output			output signal pointer
 * @param 	samples			number of samples
 */
void Compressor::apply(float *input, float *output, int samples) {
    for (int sample = 0; sample < samples; sample++) {
        output[sample] = input[sample];
    }
}
