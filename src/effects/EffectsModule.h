#ifndef EFFECTSMODULE_H
#define EFFECTSMODULE_H

// Files
#include "Compressor.h"
// Classes inheritance
#define CLASSES public Compressor
// Default constructor
#define INITIALIZERS Compressor::Compressor()

/**
 * @class	EffectModule
 * @author	Andrés González Fornell
 * @brief	It contains (by inheritance) all effects classes.
 */
class EffectsModule: public Compressor {
public:
	EffectsModule();
	~EffectsModule();
};

#endif // EFFECTSMODULE_H
