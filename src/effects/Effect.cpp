#include "Effect.h"

/**
 * @brief   Effect constructor.
 */
Effect::Effect(effectname effect, std::string parameters) : INITIALIZERS {
    consolelog("Effect",LogType::progress,"Effect object is created");
}

/**
 * @brief   Effect destructor.
 */
Effect::~Effect() {
    consolelog("Effect",LogType::progress,"Effect object is created");
}

/**
 * @brief   It applies the selected effect to the input and sets the result into output variable.
 * @param   input           input data pointer
 * @param   output          output data pointer
 * @param   samples         number of samples
 * @return  true if it was successful
 */
bool Effect::apply(float *input, float *output, int samples) {
    switch (this->effect) {
    case effectname::Compressor:
        Compressor::apply(input, output, samples);
        break;
    default:
        consolelog("Effect", LogType::error, "selected effect is not available");
        for (int sample = 0; sample < samples; sample++) {
            output[sample] = input[sample];
        }
        return false;
    }
    return true;
}
