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
// ----------------------------------------------------------------------
// 4. DEFAULT CONSTRUCTOR
// Add:         EffectClassName::EffectClassName(params)
// Description: It defines the defautl constructor from:
//                  Effect::Effect(std::string params)
//              Every line needs to have a backslash at the end except of
//              the last one.
// Example:     Compressor::Compressor(params)
#define INITIALIZERS \
    Compressor::Compressor(params), \
    Equalizer::Equalizer(params)

/**
 * @class	Effect
 * @author	Andrés González Fornell
 * @brief	Effect class. It contains (by inheritance) all effects classes.
 */
class Effect : INHERITANCE {
public:
    #define EFFECT(ID,NAME) ID,
    enum effectID {LIST};                           /**< available effects enumeration */
    effectID effect;                                /**< selected effect id */
    std::map<std::string, std::string> params;      /**< string of effect parameters */
    Effect(Effect::effectID effect, std::map<std::string, std::string> params);
    ~Effect();
    bool apply(float *input, float *output, int samples);
    static std::map<std::string,Effect::effectID> getEffects();
    static effectID getEffect(std::string effectname);
    static std::string readInfo(std::string info, std::string tag);
};

#endif // EFFECT_H
