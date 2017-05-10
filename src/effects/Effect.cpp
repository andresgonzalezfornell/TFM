#include "Effect.h"
#include "EffectBase.h"

/**
 * @brief   Effect constructor.
 * @param   effect              effect ID
 * @param   params              string of effect parameters
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
            valid += (0x61 <= iterator->first[character] && iterator->first[character] <= 0x7A);// lowercase letters
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
 * @param   input               input data pointer
 * @param   output              output data pointer
 * @param   samples             number of samples
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
        consolelog("Effect", LogType::error,
                   "selected effect is not available");
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
    std::vector<Effect::effectID> values = { LIST };
#define EFFECT(ID,NAME) NAME,
    std::vector<std::string> keys = { LIST };
    if (values.size() == keys.size()) {
        for (int index = 0; index < (int) values.size(); index++) {
            effects.insert(std::pair<std::string, Effect::effectID>(keys[index], values[index]));
        }
        return effects;
    } else {
        consolelog("Effect", LogType::error,
                   "the number of effect names (" + std::to_string(keys.size())
                   + ") does not match the number of effects ("
                   + std::to_string(values.size()) + ")");
        return std::map<std::string, Effect::effectID>();
    }
}

/**
 * @brief   It get effects type from the effect name.
 * @param   effectname          effect name string
 * @return  effect type
 * @ref     effectID
 */
Effect::effectID Effect::getEffect(std::string effectname) {
#define EFFECT(ID,NAME) NAME,
    std::vector<std::string> list = { LIST };
    for (int index = 0; index < (int) list.size(); index++) {
        if (list[index] == effectname) {
            consolelog("Effect", LogType::progress,
                       "selected #" + std::to_string(index) + ": " + effectname
                       + " as effect");
            return (Effect::effectID) index;
        }
    }
    consolelog("Effect", LogType::warning, "effect \"" + effectname + "\" is not a valid effect");
    return (Effect::effectID) 0;
}

/**
 * @brief   It get channels vector from a effect configuration file (.fx) text.
 * @param   configuration       contained text of a effect configuration file (.fx)
 * @param   size                number of channels
 * @return  channels boolean vector to select channels when applying effects
 */
std::vector<bool> Effect::getChannels(std::string configuration, int size) {
    std::map<std::string, std::string> map = Effect::getTagMap(configuration, "channels");
    std::vector<bool> channels = std::vector<bool>(size, false);
    for (int index = 0; index < (int) channels.size(); index++) {
        std::string value = map["ch" + std::to_string(index)];
        channels[index] = EffectBase::getBool(value);
    }
    return channels;
}

/**
 * @brief   It get levels vector from a effect configuration file (.fx) text.
 * @param   configuration       contained text of a effect configuration file (.fx)
 * @param   size                number of channels
 * @return  levels vector of input channels before applying effects
 */
std::vector<double> Effect::getLevels(std::string configuration, int size) {
    std::map<std::string, std::string> map = Effect::getTagMap(configuration, "levels");
    std::vector<double> levels = std::vector<double>(size, 1);
    for (std::map<std::string, std::string>::iterator iterator = map.begin();
        iterator != map.end(); iterator++) {
        if (iterator->first.find("ch") != 0 || iterator->first[2] < 0x30 || iterator->first[2] > 0x39) {
            consolelog("Effect", LogType::error, "syntax error in \"" + iterator->first + "\"");
            return levels;
        }
    }
    for (int index = 0; index < (int) levels.size(); index++) {
        std::string value = map["ch" + std::to_string(index)];
        levels[index] = std::atof(value.c_str());
    }
    return levels;
}

/**
 * @brief   It get params from a effect configuration file (.fx) text.
 * @param   configuration       contained text of a effect configuration file (.fx)
 * @return  parameters map variable valid to apply effects
 */
std::map<std::string, std::string> Effect::getParams(std::string configuration) {
    std::map<std::string, std::string> params = Effect::getTagMap(configuration, "params");
    return params;
}

/**
 * @brief   It extracts the value in a tag from a effect configuration file (.fx) text.
 * @param   configuration       contained text of a effect configuration file (.fx)
 * @param   tag                 tag name of the requested field
 * @return  contained value in the tag
 */
std::string Effect::getTag(std::string configuration, std::string tag) {
    int start = configuration.find("<" + tag + ">") + tag.size() + 2;
    int length = configuration.find("</" + tag + ">") - start;
    return configuration.substr(start, length);
    consolelog("Effect", LogType::progress, "Effect object is created");
}

/**
 * @brief   It extracts the map of values in a map-structured tag from a effect configuration file (.fx) text.
 * @param   configuration       contained text of a effect configuration file (.fx)
 * @param   tag                 tag name of the requested field
 * @return  contained map of values in the tag
 */
std::map<std::string, std::string> Effect::getTagMap(std::string configuration, std::string tag) {
    configuration = Effect::getTag(configuration, tag);
    std::map<std::string, std::string> map;
    while ((int) configuration.size() > 0) {
        std::string line = configuration.substr(0, configuration.find(";"));
        if (configuration.find(";") < configuration.size()) {
            configuration = configuration.substr(configuration.find(";") + 1);
        } else {
            configuration = "";
        }
        // Control characters deletion
        std::string newline = "";
        for (int character = 0; character < (int) line.size(); character++) {
            if (line[character] >= 0x20) {
                newline += line[character];
            }
        }
        line = newline;
        if (line.find("=") < line.size()) {
            // Key
            std::string key = line.substr(0, line.find("="));
            while (key[0] == ' ') {
                key = key.substr(1);
            }
            while (key[key.size() - 1] == ' ') {
                key = key.substr(0, key.size() - 1);
            }
            // Value
            std::string value = line.substr(line.find("=") + 1, line.find(";") - line.find("="));
            // Spaces deletion at the beggining
            while (value[0] == ' ') {
                value = value.substr(1);
            }
            // Spaces deletion at the end
            while (value[value.size() - 1] == ' ') {
                value = value.substr(0, value.size() - 1);
            }
            map.insert(std::pair<std::string, std::string>(key, value));
        } else if (line != "") {
            consolelog("Effect", LogType::error, "syntax error in \"" + line + "\" (\"=\" is missed)");
        }
    }
    return map;
}

/**
 * @brief   EffectBase constructor.
 * @param   params              string of effect parameters
 */
EffectBase::EffectBase(std::map<std::string, std::string> params) {
    this->params = params;
}

/**
 * @brief   It parse a parameter value to double
 * @param   param               parameter value
 * @return  value as integer
 */
int EffectBase::getInt(std::string param) {
    return std::atoi(param.c_str());
}

/**
 * @brief   It parse a parameter value to integer
 * @param   param               parameter value
 * @return  value
 */
double EffectBase::getDouble(std::string param) {
    return std::atof(param.c_str());
}

/**
 * @brief   It parse a parameter value to string
 * @param   param               parameter value
 * @return  value
 */
std::string EffectBase::getString(std::string param) {
    // Quotes deletion
    if (param[0] == '"' && param[param.size()-1] == '"') {
        param = param.substr(1, param.size() - 2);
        return param;
    } else {
        consolelog("EffectBase", LogType::error, "param = " + param + " is not a string");
        return NULL;
    }
}

/**
 * @brief   It parse a parameter value to bool
 * @param   param               parameter value
 */
bool EffectBase::getBool(std::string param) {
    if (param == "true" || param == "TRUE" || param == "True" || param == "1") {
        return true;
    } else {
        return false;
    }
}
