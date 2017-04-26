#ifndef EFFECT_H
#define EFFECT_H

// System libraries
#include "stdlib.h"
// Classes and local files
#include "../tools/Logger.h"

// Files
#include "Compressor.h"
// List (separated by commas)
#define LIST Compressor
// Classes inheritance (separated by commas)
#define INHERITANCE public Compressor
// Default constructor (separated by commas)
#define INITIALIZERS Compressor::Compressor()

/**
 * @class	Effect
 * @author	Andrés González Fornell
 * @brief	Effect class. It contains (by inheritance) all effects classes.
 */
class Effect : INHERITANCE {
public:
    enum effectname {LIST};     /**< available effects enumeration */
    effectname effect;          /**< selected effect id */
    Effect(effectname effect, std::string parameters);
    ~Effect();
    bool apply(float *input, float *output, int samples);
};

#endif // EFFECT_H
