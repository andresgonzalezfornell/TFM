// Header
#include "sac_encoder.h"
// System libraries
#include "string.h"
// SAC libraries
#include "libtsp.h"
#include "sac_enc.h"
#include "bitstream.h"
#include "sac_bd_embedder.h"

/**
 * @brief   It performs the SAC encoder.
 * @param   input_filename      filename of the multichannel input audio file
 * @param   output_filename     filename of the downmix output audio file (it will be automatically created)
 * @param   bitstream_filename  filename of the bitstream output file or "buried" (it will be automatically created)
 * @param   fs                  audio sampling frequency [Hz]
 * @param   tree                tree config: 5151 (mono), 5152 (mono), 525 (stereo) (5151 by default)
 * @param   timeslots           times slots: 16 or 32 (32 by default)
 * @return  error message (NULL if the encoding has succeded)
 */
char *sac_encode(const char *input_filename, const char *output_filename, const char *bitstream_filename, double fs, int tree, int timeslots) {
    // Arguments
    int buried = strcmp(bitstream_filename,"buried")==0;
    int output_channels = 0;
    switch(tree) {
    default:
        tree=5151;
    case 5151:
    case 5152:
        output_channels=1;
        break;
    case 525:
        output_channels=2;
        break;
    }
    if(timeslots!=16 || timeslots!=32) {
        timeslots = 32;
    }
    // Initialization
    AFILE *input;
    AFILE *output;
    Stream bitstream;
    long int input_channels;
    long int samples;
    InitStream(&bitstream, (char *)bitstream_filename, STREAM_WRITE);
    input = AFopnRead(input_filename, &samples, &input_channels, &fs, NULL);
    if(input_channels != 6) {
        return "input audio file must have 6 channels";
    }
    if(fs != 44100) {
        return "input audio file must have 44.1 kHz sampling rate";
    }
    output = AFopnWrite(output_filename, 2+16, 5 , output_channels, fs, NULL);
    // Encoding
    int buffersize = 0;
    int offset = 0;
    float input_buffer[MAX_INPUT_CHANNELS * MAX_BUFFER_SIZE];
    float output_buffer[MAX_INPUT_CHANNELS * MAX_BUFFER_SIZE];
    unsigned char *bitstream_buffer;
    int bitstream_length;
    int frame = 0;
    tBdEmbedder *embedder = NULL;
    tSpatialEnc *encoder = SpatialEncOpen(tree, timeslots, &buffersize, &bitstream);
    if(encoder == NULL) {
        return "failed to open spatial encoder";
    }
    if(buried) {
        embedder = BdEmbedderOpen(output_channels, timeslots);
        if(embedder == NULL) {
            return "failed to open buried data embedder";
        }
    }
    while(offset < samples) {
        AFfReadData(input, offset, input_buffer, buffersize * 6);
        SpatialEncApply(encoder, input_buffer, output_buffer, &bitstream);
        if(buried) {
            GetFirstSection(&bitstream, &bitstream_buffer, &bitstream_length);
            if(bitstream_buffer != 0) {
                BdEmbedderApply(embedder, bitstream_buffer, bitstream_length, (frame == 0), output_buffer);
            }
            DeleteFirstSection(&bitstream);
        }
        AFfWriteData(output, output_buffer, buffersize * output_channels);
        offset += buffersize * 6;
        fflush(stdout);
        frame++;
    }
    // Closing
    SpatialEncClose(encoder);
    if(buried) {
        BdEmbedderClose(embedder);
    }
    CloseStream(&bitstream);
    if(input != NULL) {
        AFclose(input);
    }
    if(output != NULL) {
        AFclose(output);
    }
    return NULL;
}
