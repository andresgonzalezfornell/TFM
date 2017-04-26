#include "Compressor.h"

/**
 * @brief   Compressor constructor.
 */
Compressor::Compressor() {

}

void Compressor::apply(float *input, float *output, int samples) {
    for (int sample = 0; sample < samples; sample++) {
        output[sample] = input[sample];
    }
}
