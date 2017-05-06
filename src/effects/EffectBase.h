#ifndef EFFECTBASE_H
#define EFFECTBASE_H

// System libraries
#include "stdlib.h"
#include "vector"
#include "map"
// Classes and local files
#include "../tools/Logger.h"

#define AS_EFFECT : public EffectBase
#define EFFECT_CONSTRUCTOR : EffectBase::EffectBase(params)

/**
 * @class	EffectBase
 * @author	Andrés González Fornell
 * @brief	Effect base class.
 */
class EffectBase {
public:
    EffectBase(std::map<std::string, std::string> param);
private:
    std::map<std::string, std::string> params;      /**< string of effect parameters */
};

#endif // EFFECTBASE_H
