// Header
#include "sac_decoder.h"
// SAC library
#include "spatial_frontend.h"

char *error = "";

/**
 * @brief   It adds a char string to the char string variable error to s.
 * @param   s                   char pointer to exit message
 */
void myexit(char *s) {
    // End line
    error = (char *) malloc(1 + strlen(error) + strlen("\n"));
    strcpy(error, error);
    strcat(error, "\n");
    free(error);
    // Message
    error = (char *) malloc(1 + strlen(error) + strlen(s));
    strcpy(error, error);
    strcat(error, s);
    free(error);
}

/**
 * @brief   It performs the SAC decoding.
 * @param   input_filename      filename of the downmix input audio file
 * @param   output_filename     filename of the multichannel output audio file (it will be automatically created)
 * @param   bitstream_filename  filename of the bitstream file (not present in blind upmix case)
 * @param   fs                  audio sampling frequency [Hz]
 * @param   upmixtype           upmix type              0: normal       1: blind        2: binaural     3: stereo
 * @param   decodingtype        decoding type           0: low          1: high
 * @param   binauralquality     binaural upmix quality  0: parametric   1: filtering
 * @param   hrtfmodel           HRTF model              0: kemar        1: vast         2: mps_vt
 * @return  error message (NULL if the decoding has succeded)
 */
char *sac_decode(const char *input_filename, const char *output_filename,
                 const char *bitstream_filename, double fs, int upmixtype,
                 int decodingtype, int binauralquality, int hrtfmodel) {
    /** @cond */
    // Arguments
    int buried = strcmp(bitstream_filename, "buried") == 0;
    BITSTREAM_SOURCE bitstream_type;
    if (buried) {
        bitstream_type = BS_BURIED;
    } else {
        bitstream_type = BS_FILE;
    }
    // Initialization
    long int input_channels;
    float* input_samples;
    float* input_interleaved;
    BD_INPUT_BUFFER input_buffer;
    HANDLE_BYTE_READER bitstream = NULL;
    HANDLE_FILE_READER bitstream_reader = NULL;
#ifdef HRTF_DYNAMIC_UPDATE
    HANDLE_HRTF_READER hrtf_reader = NULL;
    HANDLE_HRTF_SOURCE hrtf_source = NULL;
#endif
    spatialDec* decoder;
    int done = 0; // flag to exit decoding
    long int samples;
    int samples_read;
    int samplespercall;
    int qmfbands = 0;
    int timeslots;
    long int offset = 0;
    HANDLE_BURIED_DATA_READER bdreader = NULL;
    SAC_POLYPHASE_ANA_FILTERBANK *filterbank[MAX_INPUT_CHANNELS] = { 0 };
    float** qmf_real[MAX_INPUT_CHANNELS];
    float** qmf_imag[MAX_INPUT_CHANNELS];
    float** input_pointers[2 * MAX_INPUT_CHANNELS];
    AFILE* input = 0;
    if (input_filename) {
        input = AFopnRead(input_filename, &samples, &input_channels, &fs, 0);
    } else {
        return "input file name was not given";
    }
    if (bitstream_type == BS_FILE) {
        bitstream_reader = FileReaderOpen((char *)bitstream_filename);
        bitstream = FileReaderGetByteReader(bitstream_reader);
    }
    // QMF bands
#ifdef PARTIALLY_COMPLEX
    for (int band=0; band < MAX_INPUT_CHANNELS; band++) {
        qmf_real[band] = (float **) calloc (MAX_TIME_SLOTS, sizeof (float*));
        for (int timeslot=0; timeslot < MAX_TIME_SLOTS; timeslot++) {
            qmf_real[band][timeslot] = (float *) calloc (MAX_QMF_BANDS, sizeof (float));
        }
        input_pointers[band] = qmf_real[band];
        (qmf_imag);
    }
#else
    for (int band = 0; band < MAX_INPUT_CHANNELS; band++) {
        qmf_real[band] = (float **) calloc(MAX_TIME_SLOTS, sizeof(float*));
        qmf_imag[band] = (float **) calloc(MAX_TIME_SLOTS, sizeof(float*));
        for (int timeslot = 0; timeslot < MAX_TIME_SLOTS; timeslot++) {
            qmf_real[band][timeslot] = (float *) calloc(MAX_QMF_BANDS,
                                                        sizeof(float));
            qmf_imag[band][timeslot] = (float *) calloc(MAX_QMF_BANDS,
                                                        sizeof(float));
        }
        input_pointers[2 * band] = qmf_real[band];
        input_pointers[2 * band + 1] = qmf_imag[band];
    }
#endif
    if (setjmp(g_JmpBuf) == 0) {
    } else {
        return "error because of buffer jumper";
    }
    if (fs < 27713.0) {
        qmfbands = 32;
    } else if (fs >= 55426.0) {
        qmfbands = 128;
    } else {
        qmfbands = 64;
    }
    // Decoding
    if (bitstream_type == BS_BURIED) {
        float input_header[BD_HEADER_LENGTH * MAX_INPUT_CHANNELS];
        int input_header_samples = BD_HEADER_LENGTH * input_channels;
        int samples_remaining;
        bdreader = BdReaderOpen(qmfbands);
        bitstream = BdReaderGetByteReader(bdreader);
        input_buffer.channels = input_channels;
        input_buffer.sampleFormat = input->Format;
        input_buffer.bitsPerSample = input->NbS;
        input_buffer.data = input_header;
        offset += (samples_read = AFfReadData(input, offset, input_header,
                                              input_header_samples));
        if (samples_read != input_header_samples) {
            done = 1;
        }
        if (BdReaderGetFrameSize(bdreader, &input_buffer, &timeslots) != 0) {
            done = 1;
        }
        samplespercall = input_channels * qmfbands * timeslots;
        input_samples = (float*) calloc(sizeof(float), samplespercall);
        input_buffer.data = input_samples;
        memcpy(input_samples, input_header,
               sizeof(float) * input_header_samples);
        samples_remaining = samplespercall - input_header_samples;
        offset += (samples_read = AFfReadData(input, offset,
                                              &input_samples[input_header_samples], samples_remaining));
        if (samples_read == samples_remaining) {
            if (BdReaderParseFrame(bdreader, &input_buffer) != 0) {
                done = 1;
            }
        } else {
            done = 1;
        }
    }
#ifdef HRTF_DYNAMIC_UPDATE
    if (hrtfmodel == 3) {
        hrtf_reader = HrtfReaderOpen(hrtf_sourceFileName);
        hrtf_source = HrtfReaderGetHrtfSource(hrtf_reader);
    }
#endif
    decoder = SpatialDecOpen(bitstream, (char*)output_filename, (int) fs,
                             (int) input_channels, &timeslots, qmfbands, decodingtype, upmixtype,
                             hrtfmodel,
                         #ifdef HRTF_DYNAMIC_UPDATE
                             hrtf_source,
                         #endif
                             binauralquality);
    SacDecInitAnaFilterbank(NULL, qmfbands);
    for (int channel = 0; channel < input_channels; channel++) {
        SacDecOpenAnaFilterbank(&filterbank[channel]);
    }
    input_interleaved = (float*) calloc(sizeof(float), qmfbands);
    if (bitstream_type != BS_BURIED) {
        samplespercall = input_channels * qmfbands * timeslots;
        input_samples = (float*) calloc(sizeof(float), samplespercall);
    }
    while (!done) {
        if ((bitstream_type != BS_BURIED) || (frameCounter > 0)) {
            offset += (samples_read = AFfReadData(input, offset, input_samples,
                                                  samplespercall));
            if (samples_read == samplespercall) {
                if (bitstream_type == BS_BURIED) {
                    if (BdReaderParseFrame(bdreader, &input_buffer) != 0) {
                        done = 1;
                    }
                    if (BdReaderEndOfStream(bdreader)) {
                        done = 1;
                    }
                }
            } else
                done = 1;
        }
        if ((bitstream_type == BS_FILE) && FileReaderEof(bitstream_reader)) {
            done = 1;
        }
#ifdef HRTF_DYNAMIC_UPDATE
        if ((hrtfModel == 3) && HrtfReaderEof(hrtf_reader)) {
            done = 1;
        }
#endif
        if ((!done) && (setjmp(g_JmpBuf) == 0)) {
            if (upmixtype != 1) {
                SpatialDecParseFrame(decoder);
            }
#ifdef DMX_GAIN_FIX
            BinauralDoParallelReverb(decoder, input_samples, 32768.0f);
#else
            BinauralDoParallelReverb(ourDec, inSamples,
                                     32768.0f * SpatialGetClipProtectGain(ourDec));
#endif
            for (int channel = 0; channel < input_channels; channel++) {
                for (int timeslot = 0; timeslot < timeslots; timeslot++) {
                    for (int band = 0; band < qmfbands; band++) {
#ifdef DMX_GAIN_FIX
                        if (upmixtype == 2 || upmixtype == 3) {
                            input_interleaved[band] = input_samples[input_channels* (timeslot*qmfbands+band)+channel] * 32768.0f;
                        } else {
#endif
                            input_interleaved[band] = input_samples[input_channels
                                    * (timeslot * qmfbands + band) + channel]
                                    * 32768.0f * SpatialGetClipProtectGain(decoder);
                        }
                    }
#ifdef PARTIALLY_COMPLEX
                    SacCalculateAnaFilterbank( filterbank[channel],
                                               inSamplesDeinterleaved,
                                               inPointers[channel][ts],
                                               NULL );
#else
                    SacDecCalculateAnaFilterbank(filterbank[channel],
                                                 input_interleaved,
                                                 input_pointers[2 * channel][timeslot],
                            input_pointers[2 * channel + 1][timeslot]);
#endif
                }
            }
            SpatialDecApplyFrame(decoder, timeslots, input_pointers);
            frameCounter++;
        }
        else {
            break;
        }
    }
    // Closing
    if (input_samples != NULL) {
        free(input_samples);
    }
    if (input_interleaved != NULL) {
        free(input_interleaved);
    }
    for (int channel = 0; channel < input_channels; channel++) {
        SacDecCloseAnaFilterbank(filterbank[channel]);
    }
    SpatialDecClose( decoder);
    if (bitstream_type == BS_FILE) {
        FileReaderClose(bitstream_reader);
    }
    if (bitstream_type == BS_BURIED) {
        BdReaderClose(bdreader);
    }
#ifdef HRTF_DYNAMIC_UPDATE
    if (hrtfModel == 3) {
        HrtfReaderClose(hrtf_reader);
    }
#endif
    AFclose( input);
#ifdef PARTIALLY_COMPLEX
    for (i=0; i < MAX_INPUT_CHANNELS; i++) {
        for (j=0; j < MAX_TIME_SLOTS; j++) {
            if (qmfInReal[i][j] != NULL) {
                free(qmfInReal[i][j]);
            }
        }
        if (qmfInReal[i] != NULL) {
            free(qmfInReal[i]);
        }
    }
#else
    for (int band = 0; band < MAX_INPUT_CHANNELS; band++) {
        for (int timeslot = 0; timeslot < MAX_TIME_SLOTS; timeslot++) {
            if (qmf_real[band][timeslot] != NULL) {
                free(qmf_real[band][timeslot]);
            }
            if (qmf_imag[band][timeslot] != NULL) {
                free(qmf_imag[band][timeslot]);
            }
        }
        if (qmf_real[band] != NULL) {
            free(qmf_real[band]);
        }
        if (qmf_imag[band] != NULL) {
            free(qmf_imag[band]);
        }
    }
#endif
    if (strcmp(error,"")==0) {
        return NULL;
    } else {
        return error;
    }
    /** @endcond */
}
