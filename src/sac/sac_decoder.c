// Header
#include "sac_decoder.h"
// SAC library
#include "spatial_frontend.h"

char *error = "";

/**
 * @brief   It adds a char string to the char string variable error to s
 * @param s
 */
void myexit(char *s) {
    // End line
    error = (char *)malloc(1+strlen(error)+strlen("\n"));
    strcpy(error, error);
    strcat(error, "\n");
    // Message
    error = (char *)malloc(1+strlen(error)+strlen(s));
    strcpy(error, error);
    strcat(error, s);
}

/**
 * @brief   It performs the SAC decoder.
 * @return  0 unless errors occurred
 */
int sac_decode() {
	spatialDec* ourDec;
	long int nSamples;
	long int nChannels;
	double sampleFreq;
	int qmfBands = 0;
	int nTimeSlots;
	AFILE* pcmIn = 0;
	HANDLE_FILE_READER fileReader = NULL;
	HANDLE_BURIED_DATA_READER bdReader = NULL;
	BD_INPUT_BUFFER bdBuffer;
	HANDLE_BYTE_READER bitstream = NULL;
#ifdef HRTF_DYNAMIC_UPDATE
	HANDLE_HRTF_READER hrtfReader = NULL;
	HANDLE_HRTF_SOURCE hrtfSource = NULL;
#endif
	int done = 0;
	int samplesToReadPerCall;
	int samplesRead;
	float* inSamples;
	float* inSamplesDeinterleaved;
	int i;
	int j;
	int channel;
	int ts;
	long int offset = 0;
	SAC_POLYPHASE_ANA_FILTERBANK *filterbank[MAX_INPUT_CHANNELS] = { 0 };
	float** qmfInReal[MAX_INPUT_CHANNELS];
	float** qmfInImag[MAX_INPUT_CHANNELS];
	float** inPointers[2 * MAX_INPUT_CHANNELS];
#ifdef PARTIALLY_COMPLEX
	for (i=0; i < MAX_INPUT_CHANNELS; i++) {
		qmfInReal[i] = (float **) calloc (MAX_TIME_SLOTS, sizeof (float*));
		for (j=0; j < MAX_TIME_SLOTS; j++) {
			qmfInReal[i][j] = (float *) calloc (MAX_QMF_BANDS, sizeof (float));
		}
		inPointers[i] = qmfInReal[i];
		(qmfInImag);
	}
#else
	for (i = 0; i < MAX_INPUT_CHANNELS; i++) {
		qmfInReal[i] = (float **) calloc(MAX_TIME_SLOTS, sizeof(float*));
		qmfInImag[i] = (float **) calloc(MAX_TIME_SLOTS, sizeof(float*));
		for (j = 0; j < MAX_TIME_SLOTS; j++) {
			qmfInReal[i][j] = (float *) calloc(MAX_QMF_BANDS, sizeof(float));
			qmfInImag[i][j] = (float *) calloc(MAX_QMF_BANDS, sizeof(float));
		}
		inPointers[2 * i] = qmfInReal[i];
		inPointers[2 * i + 1] = qmfInImag[i];
	}
#endif
	if (setjmp(g_JmpBuf) == 0) {
	} else {
		exit(-1);
	}

	if (pcmInFile)
		pcmIn = AFopnRead(pcmInFile, &nSamples, &nChannels, &sampleFreq, 0);
	if (bitstreamSource == BS_FILE) {
		fileReader = FileReaderOpen(spatialBitstreamFile);
		bitstream = FileReaderGetByteReader(fileReader);
	}

	if (sampleFreq < 27713.0) {
		qmfBands = 32;
	} else if (sampleFreq >= 55426.0) {
		qmfBands = 128;
	} else {
		qmfBands = 64;
	}

	if (bitstreamSource == BS_BURIED) {
		float header[BD_HEADER_LENGTH * MAX_INPUT_CHANNELS];
		int headerSamples = BD_HEADER_LENGTH * nChannels;
		int samplesToRead;

		bdReader = BdReaderOpen(qmfBands);
		bitstream = BdReaderGetByteReader(bdReader);

		bdBuffer.channels = nChannels;
		bdBuffer.sampleFormat = pcmIn->Format;
		bdBuffer.bitsPerSample = pcmIn->NbS;
		bdBuffer.data = header;

		offset += (samplesRead = AFfReadData(pcmIn, offset, header,
				headerSamples));
		if (samplesRead != headerSamples)
			done = 1;

		if (BdReaderGetFrameSize(bdReader, &bdBuffer, &nTimeSlots) != 0)
			done = 1;

		samplesToReadPerCall = nChannels * qmfBands * nTimeSlots;
		inSamples = (float*) calloc(sizeof(float), samplesToReadPerCall);

		bdBuffer.data = inSamples;

		memcpy(inSamples, header, sizeof(float) * headerSamples);
		samplesToRead = samplesToReadPerCall - headerSamples;

		offset += (samplesRead = AFfReadData(pcmIn, offset,
				&inSamples[headerSamples], samplesToRead));
		if (samplesRead == samplesToRead) {
			if (BdReaderParseFrame(bdReader, &bdBuffer) != 0)
				done = 1;
		} else
			done = 1;
	}

#ifdef HRTF_DYNAMIC_UPDATE
	if (hrtfModel == 3) {
		hrtfReader = HrtfReaderOpen(hrtfSourceFileName);
		hrtfSource = HrtfReaderGetHrtfSource(hrtfReader);
	}
#endif

	ourDec = SpatialDecOpen(bitstream, spatialPcmOutFile, (int) sampleFreq,
			(int) nChannels, &nTimeSlots, qmfBands, decType, upmixType,
			hrtfModel,
#ifdef HRTF_DYNAMIC_UPDATE
			hrtfSource,
#endif
			binauralQuality);

	SacDecInitAnaFilterbank(NULL, qmfBands);
	for (channel = 0; channel < nChannels; channel++)
		SacDecOpenAnaFilterbank(&filterbank[channel]);

	inSamplesDeinterleaved = (float*) calloc(sizeof(float), qmfBands);

	if (bitstreamSource != BS_BURIED) {
		samplesToReadPerCall = nChannels * qmfBands * nTimeSlots;
		inSamples = (float*) calloc(sizeof(float), samplesToReadPerCall);
	}

	while (!done) {

		if ((bitstreamSource != BS_BURIED) || (frameCounter > 0)) {
			offset += (samplesRead = AFfReadData(pcmIn, offset, inSamples,
					samplesToReadPerCall));
			if (samplesRead == samplesToReadPerCall) {
				if (bitstreamSource == BS_BURIED) {
					if (BdReaderParseFrame(bdReader, &bdBuffer) != 0)
						done = 1;
					if (BdReaderEndOfStream(bdReader))
						done = 1;
				}
			} else
				done = 1;
		}

		if ((bitstreamSource == BS_FILE) && FileReaderEof(fileReader))
			done = 1;

#ifdef HRTF_DYNAMIC_UPDATE
		if ((hrtfModel == 3) && HrtfReaderEof(hrtfReader)) done = 1;
#endif

		if ((!done) && (setjmp(g_JmpBuf) == 0)) {

			if (upmixType != 1) {
				SpatialDecParseFrame(ourDec);
			}

#ifdef DMX_GAIN_FIX
			BinauralDoParallelReverb(ourDec, inSamples, 32768.0f);
#else
			BinauralDoParallelReverb(ourDec, inSamples,
					32768.0f * SpatialGetClipProtectGain(ourDec));
#endif

			for (channel = 0; channel < nChannels; channel++) {

				for (ts = 0; ts < nTimeSlots; ts++) {
					for (i = 0; i < qmfBands; i++) {
#ifdef DMX_GAIN_FIX
						if (upmixType == 2 || upmixType == 3)
						inSamplesDeinterleaved[i] = inSamples[nChannels* (ts*qmfBands+i)+channel] * 32768.0f;
						else
#endif
						inSamplesDeinterleaved[i] = inSamples[nChannels
								* (ts * qmfBands + i) + channel] * 32768.0f
								* SpatialGetClipProtectGain(ourDec);
					}

#ifdef PARTIALLY_COMPLEX
					SacCalculateAnaFilterbank( filterbank[channel],
							inSamplesDeinterleaved,
							inPointers[channel][ts],
							NULL );
#else
					SacDecCalculateAnaFilterbank(filterbank[channel],
							inSamplesDeinterleaved, inPointers[2 * channel][ts],
							inPointers[2 * channel + 1][ts]);
#endif
				}
			}
			SpatialDecApplyFrame(ourDec, nTimeSlots, inPointers);

			frameCounter++;

		} else {
			break;
		}
	}

	if (inSamples != NULL)
		free(inSamples);
	if (inSamplesDeinterleaved != NULL)
		free(inSamplesDeinterleaved);

	for (channel = 0; channel < nChannels; channel++)
		SacDecCloseAnaFilterbank(filterbank[channel]);

	SpatialDecClose(ourDec);
	if (bitstreamSource == BS_FILE)
		FileReaderClose(fileReader);
	if (bitstreamSource == BS_BURIED)
		BdReaderClose(bdReader);
#ifdef HRTF_DYNAMIC_UPDATE
	if (hrtfModel == 3) HrtfReaderClose(hrtfReader);
#endif
	AFclose(pcmIn);

#ifdef PARTIALLY_COMPLEX
	for (i=0; i < MAX_INPUT_CHANNELS; i++) {
		for (j=0; j < MAX_TIME_SLOTS; j++) {
			if (qmfInReal[i][j] != NULL) free(qmfInReal[i][j]);
		}
		if (qmfInReal[i] != NULL) free(qmfInReal[i]);
	}
#else
	for (i = 0; i < MAX_INPUT_CHANNELS; i++) {
		for (j = 0; j < MAX_TIME_SLOTS; j++) {
			if (qmfInReal[i][j] != NULL)
				free(qmfInReal[i][j]);
			if (qmfInImag[i][j] != NULL)
				free(qmfInImag[i][j]);
		}
		if (qmfInReal[i] != NULL)
			free(qmfInReal[i]);
		if (qmfInImag[i] != NULL)
			free(qmfInImag[i]);
	}
#endif
	return 0;
}
