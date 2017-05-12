#ifndef EFFECTBASE_H
#define EFFECTBASE_H

// System libraries
#include "stdlib.h"
#include "vector"
#include "map"
// Classes and local files
#include "../process/AudioSignal.h"
#include "../tools/Logger.h"

#define AS_EFFECT : public EffectBase
#define AS_EFFECT_CONSTRUCTOR : EffectBase::EffectBase()

/**
 * @class	EffectBase
 * @author	Andrés González Fornell
 * @brief	Effect base class.
 */
class EffectBase {
public:
    static std::map<std::string, std::string> params; /**< string of effect parameters */
    EffectBase();
    static int getInt(std::string param);
    static double getDouble(std::string param);
    static std::string getString(std::string param);
    static bool getBool(std::string param);
private:
};

#endif // EFFECTBASE_H
