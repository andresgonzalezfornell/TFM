#include "Effect.h"
#include "EffectBase.h"

/**
 * @brief   Effect constructor.
 * @param   effect          effect ID
 * @param   params          string of effect parameters
 */
Effect::Effect(Effect::effectID effect, std::string params) : INITIALIZERS {
    this->params = params;
    this->effect = effect;
    consolelog("Effect",LogType::progress,"Effect object is created");
}

/**
 * @brief   Effect destructor.
 */
Effect::~Effect() {
    consolelog("Effect",LogType::progress,"Effect object is deleted");
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
EffectBase::EffectBase(std::string params) {
    this->params = params;
}

/**
 * @brief   It gets a parameter from the parameters effect string.
 * @param   name            parameter name
 * @return  parameter value
 */
double EffectBase::getParam(std::string name) {
    std::string data = this->params;
    int start = 0;
    if (data.find(name + " = ") < data.size()) {
        start = data.find(name + " = ") + name.size() + 3;
    } else if (data.find(name + " =") < data.size()) {
        start = data.find(name + " =") + name.size() + 2;
    } else if (data.find(name + "= ") < data.size()) {
        start = data.find(name + "= ") + name.size() + 2;
    } else if (data.find(name + "=") < data.size()){
        start = data.find(name + "=") + name.size() + 1;
    } else {
        consolelog("EffectBase", LogType::error, "parameter \"" + name + "\" could not be found");
        return 0;
    }
    data = data.substr(start);
    int length = data.find(" ");
    if ((int)data.find("\n") < length) {
        length = data.find("\n");
    }
    if ((int)data.find("\n") < length) {
        length = data.find("\t");
    }
    data = data.substr(0,length);
    double value = atof(data.c_str());
    return value;
}
