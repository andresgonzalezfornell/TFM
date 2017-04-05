#ifndef AUDIOFILE_H
#define AUDIOFILE_H

// System libraries
#include "stdlib.h"
// Qt libraries
#include "QFile"
#include "QByteArray"
#include "QMessageBox"
// Classes and local files
#include "../tools/Logger.h"

/**
 * @class	AudioFile
 * @author	Andrés González Fornell
 * @brief	Audio file class.
 */

class AudioFile {
public:
    /**
     * @name    Endianess
     * @brief   Endianess type.
     */
    struct Endianess {
        enum endianess {
            littleendian,                   /**< little endian */
            bigendian                       /**< big endian */
        };
    };
    /**
     * @brief   Audio file header struct.
     */
    struct Header {
        /**
         * @name    Chunk header
         * @brief   It indicates the audio format (wave).
         * @{
         */
        std::string chunkID;                /**< it contains the letters "RIFF" in ASCII form */
        unsigned long int chunksize;        /**< size of the entire file in bytes minus 8 bytes for the two fields not included in this count (ChunkID and ChunkSize) */
        std::string format;                 /**< it contains the letters "WAVE" */
        /** @} */
        /**
         * @name    Subshunk 1 header
         * @brief   It describes the format of the sound information in the data sub-chunk.
         * @{
         */
        std::string subchunk1ID;            /**< it contains the letters "fmt " */
        unsigned long int subchunk1size;    /**< size of the rest of the subchunk (16 for PCM) */
        unsigned long int audioformat;      /**< PCM = 1 (linear quantization) values others than 1 indicate some form of compression */
        unsigned long int numchannels;      /**< number of channels */
        unsigned long int samplerate;       /**< sample rate */
        unsigned long int byterate;         /**< byte rate (= samplerate * numchannels * bitspersample/8) */
        unsigned long int blockalign;       /**< number of bytes for one sample including all channels (= numchannels * bitspersample/8) */
        unsigned long int bitspersample;    /**< number of bits per sample */
        /** @} */
        /**
         * @name    Subshunk 2 header
         * @brief   It indicates the size of the sound information.
         * @{
         */
        std::string subchunk2ID;            /**< it contains the letters "data */
        unsigned long int subchunk2size;    /**< size of ther rest of the subchunk (it is the size of the data) */
        /** @} */
    };
    Header header;                          /**< audio file header */
    int cursor;                             /**< audio file reading cursor [Bytes] */
    float duration;                         /**< audio file duration [s] */
    AudioFile();
    AudioFile(std::string filepath);
    ~AudioFile();
    std::string getFilepath();
    QByteArray readData(int length);
    unsigned long readDataNumber(int length,Endianess::endianess endianess);
    void readHeader();
    bool exists();
private:
    QFile *file;                            /**< file object */
};

#endif // AUDIOFILE_H
