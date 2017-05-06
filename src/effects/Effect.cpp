#include "Effect.h"
#include "EffectBase.h"

/**
 * @brief   Effect constructor.
 * @param   effect          effect ID
 * @param   params          string of effect parameters
 */
Effect::Effect(Effect::effectID effect, std::map<std::string, std::string> params) : INITIALIZERS {
    // Deleting banned characters for param
    for (std::map<std::string, std::string>::iterator iterator = params.begin(); iterator != params.end(); iterator++) {
        std::string param = "";
        for (int character = 0; character < (int)iterator->first.size(); character++) {
            bool valid = false;
            if (character > 0) {
                valid += (0x30 <= iterator->first[character] && iterator->first[character] <= 0x39); // numbers
            }
            valid += (0x41 <= iterator->first[character] && iterator->first[character] <= 0x5A); // uppercase letters
            valid += (0x61 <= iterator->first[character] && iterator->first[character] <= 0x7A); // lowercase letters
            if (valid) {
                param += iterator->first[character];
            } else {
                consolelog("Effect", LogType::warning, "character \"" + iterator->first.substr(character,1) + "\" is not allowed as parameter name in \"" + iterator->first + "\" and it will be removed");
            }
        }
        this->params.insert(std::pair<std::string, std::string>(param, iterator->second));
    }
    this->effect = effect;
}

/**
 * @brief   Effect destructor.
 */
Effect::~Effect() {
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
    case effectID::Compressor:
        Compressor::apply(input, output, samples);
        break;
    case effectID::Equalizer:
        Equalizer::apply(input, output, samples);
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

/**
 * @brief   It gets the list of available effects.
 * @return  map of available effects
 */
std::map<std::string, Effect::effectID> Effect::getEffects() {
    std::map<std::string, Effect::effectID> effects;
#define EFFECT(ID,NAME) ID,
    std::vector<Effect::effectID> values = {LIST};
#define EFFECT(ID,NAME) NAME,
    std::vector<std::string> keys = {LIST};
    if (values.size() == keys.size()) {
        for (int index = 0; index < (int)values.size(); index++) {
            effects.insert(std::pair<std::string, Effect::effectID>(keys[index],values[index]));
        }
        return effects;
    } else {
        consolelog("Effect", LogType::error, "the number of effect names (" + std::to_string(keys.size()) + ") does not match the number of effects (" + std::to_string(values.size()) + ")");
        return std::map<std::string, Effect::effectID>();
    }
}

/**
 * @brief   It extracts a field from a effect information file (.fx) text
 * @param   info            containned text of a effect information file (.fx)
 * @param   tag             tag name of the requested field
 * @return  value of the field
 */
std::string Effect::readInfo(std::string info, std::string tag) {
    int start = info.find("<" + tag + ">") + tag.size() + 2;
    int length = info.find("</" + tag + ">") - start;
    return info.substr(start, length);
    consolelog("Effect",LogType::progress,"Effect object is created");
}

Effect::effectID Effect::getEffect(std::string effectname) {
    #define EFFECT(ID,NAME) NAME,
    std::vector<std::string> list = {LIST};
    for (int index = 0; index < (int)list.size(); index++) {
        if (list[index] == effectname) {
            consolelog("Effect",LogType::progress, "selected #" + std::to_string(index) + ": " + effectname + " as effect");
            return (Effect::effectID)index;
        }
    }
    consolelog("Effect", LogType::warning, "effect \"" + effectname + "\" is not a valid effect");
    return (Effect::effectID)0;
}

/**
 * @brief   EffectBase constructor.
 * @param   params          string of effect parameters
 */
EffectBase::EffectBase(std::map<std::string, std::string> params) {
    this->params = params;
}
