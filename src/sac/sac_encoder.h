#ifndef SAC_ENCODER_H
#define SAC_ENCODER_H

// System libraries
#include "math.h"
#include "stdlib.h"

char *sac_encode(const char *input_filename, const char *output_filename,
		const char *bitstream_filename, double fs, int tree, int timeslots);

#endif // SAC_ENCODER_H
