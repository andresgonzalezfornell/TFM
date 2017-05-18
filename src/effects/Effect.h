#ifndef EFFECT_H
#define EFFECT_H

// FORM TO INCLUDE A NEW EFFECT
// ----------------------------------------------------------------------
// 1. FILES
// Add:         #include  "Header file"
// Description: Your effect class header should be in src/effects/.
//              Otherwise, you had to reference the header file as a
//              relative path from this folder.
// Example:     #include "Compressor.h"
#include "Compressor.h"
#include "Equalizer.h"
// ----------------------------------------------------------------------
// 2. LIST
// Add:         EFFECT(EffectClassName, "EffectClassName")
// Description: Every line needs to have a backslash at the end except of
//              the last one.
// Example:     EFFECT(Compressor, "Compressor")
#define LIST \
    EFFECT(Compressor, "Compressor") \
    EFFECT(Equalizer, "Equalizer")
// ----------------------------------------------------------------------
// 3. INHERITANCE OF CLASSES
// Add:         public EffectClassName
// Description: Every line needs to have a comma and a backslash at the
//              end except of the last one.
// Example:     public Compressor
#define INHERITANCE \
    public Compressor, \
    public Equalizer

/**
 * @class	Effect
 * @author	Andrés González Fornell
 * @brief	Effect class. It contains (by inheritance) all effects classes.
 */
class Effect: INHERITANCE {
public:
#define EFFECT(ID,NAME) ID,
    enum effectID {
        LIST
    }; /**< available effects enumeration */
    std::pair<Effect::effectID, std::string> effect; /**< selected effect name and id */
    Effect(Effect::effectID effect, int fs);
    Effect(Effect::effectID effect, std::map<std::string, std::string> params, int fs);
    ~Effect();
    void setParams(std::map<std::string, std::string> params);
    bool apply(float *input, float *output, int samples);
    std::vector<std::vector<double> > plot(std::string chart);
    static std::map<Effect::effectID, std::string> getEffects();
    static Effect::effectID getEffect(std::string effectname);
    static std::map<std::string, std::string> getParams(std::string configuration);
    static std::vector<bool> getChannels(std::string configuration, int size);
    static std::vector<double> getLevels(std::string configuration, int size);
    static std::string getTag(std::string configuration, std::string tag);
    static std::map<std::string, std::string> getTagMap(std::string configuration, std::string tag);
};

#endif // EFFECT_H
