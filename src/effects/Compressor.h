#ifndef COMPRESSOR_H
#define COMPRESSOR_H

/**
 * @class	Compressor
 * @author	Andrés González Fornell
 * @brief	Audio compressor.
 */
class Compressor {
public:
    Compressor();
    void apply(float *input, float *output, int samples);
};

#endif // COMPRESSOR_H
