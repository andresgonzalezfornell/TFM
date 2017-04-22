#ifndef AUDIOFILE_H
#define AUDIOFILE_H

// System libraries
#include "stdlib.h"
#include "fstream"
// Classes and local files
#include "../tools/Logger.h"

/**
 * @class	AudioFile
 * @author	Andrés González Fornell
 * @brief	Audio file class.
 */
class File {
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
    std::fstream *file;                     /**< file object */
    std::string filename;                   /**< file path name */
    int cursor;                             /**< audio file reading cursor [Bytes] */
    File();
    File(std::string filename);
    ~File();
    void setFilename(std::string filename);
    std::string getFilename();
    void setCursor(int cursor);
    bool exists();
    std::string readData(int length);
    unsigned long readValue(int length,Endianess::endianess endianess);
};

/**
 * @class	WAVFile
 * @author	Andrés González Fornell
 * @brief	Audio file as WAV format class.
 */
class WAVFile : public File {
public:
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
    float duration;                         /**< audio file duration [s] */
    WAVFile();
    WAVFile(std::string filename);
    void readHeader();                      /**< file object */
};

#endif // AUDIOFILE_H
