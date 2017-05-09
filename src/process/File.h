#ifndef FILE_H
#define FILE_H

// System libraries
#include "stdlib.h"
#include "cmath"
#include "fstream"
#include "sys/stat.h"
// Classes and local files
#include "../tools/Logger.h"

/**
 * @class	File
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
            littleendian, /**< little endian */
            bigendian /**< big endian */
        };
    };
    File(bool writepermission);
    File(std::string filename, bool writepermission);
    ~File();
    void setFilename(std::string filename);
    std::string getFilename();
    void setCursor(int cursor);
    int getCursor();
    int size();
    bool exists();
    char *read(int length);
    std::string readData(int length);
    void writeData(std::string data);
    unsigned readNumber(int length, Endianess::endianess endianess);
    void writeNumber(unsigned int data, int length, Endianess::endianess endianess);
private:
    std::fstream *file; /**< file object */
    std::string filename; /**< file path name */
    bool writepermission; /**< write file permission (true if it is allowed) */
    int cursor; /**< file reading cursor [Bytes] */

};

/**
 * @class	WAVFile
 * @author	Andrés González Fornell
 * @brief	Audio file as WAV format class.
 */
class WAVFile: public File {
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
        std::string chunkID; /**< it contains the letters "RIFF" in ASCII form */
        unsigned int chunksize; /**< size of the entire file in bytes minus 8 bytes for the two fields not included in this count (ChunkID and ChunkSize) */
        std::string format; /**< it contains the letters "WAVE" */
        /** @} */
        /**
         * @name    Subshunk 1 header
         * @brief   It describes the format of the sound information in the data sub-chunk.
         * @{
         */
        std::string subchunk1ID; /**< it contains the letters "fmt " */
        unsigned int subchunk1size; /**< size of the rest of the subchunk (16 for PCM) */
        unsigned int audioformat; /**< PCM = 1 (linear quantization) values others than 1 indicate some form of compression */
        unsigned int numchannels; /**< number of channels */
        unsigned int samplerate; /**< sample rate */
        unsigned int byterate; /**< byte rate (= samplerate * numchannels * bitspersample/8) */
        unsigned int blockalign; /**< number of bytes for one sample including all channels (= numchannels * bitspersample/8) */
        unsigned int bitspersample; /**< number of bits per sample */
        /** @} */
        /**
         * @name    Subshunk 2 header
         * @brief   It indicates the size of the sound information.
         * @{
         */
        std::string subchunk2ID; /**< it contains the letters "data */
        unsigned int subchunk2size; /**< size of ther rest of the subchunk (it is the size of the data) */
        /** @} */
        int size() {
            return subchunk1size + 28;
        }
    };
    Header header; /**< audio file header */
    double duration; /**< audio file duration [s] */
    WAVFile(bool writepermission);
    WAVFile(std::string filename, bool writepermission);
    WAVFile(std::string filename, int channels, int fs, int sampleformat);
    ~WAVFile();
    void setCursor(int cursor);
    int getCursor();
    int samples();
    void readHeader();
    void writeHeader();
    float readValue();
    void writeValue(float value);
};

#endif // FILE_H
