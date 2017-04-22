#include "File.h"

/**
 * @brief	AudioFile constructor.
 */
File::File() {
    this->setCursor(0);
    consolelog("AudioFile",LogType::progress,"AudioFile object is created");
}

/**
 * @brief	AudioFile constructor.
 * @param   filepath        file path
 */
File::File(std::string filename) {
    this->setFilename(filename);
    this->setCursor(0);
    consolelog("AudioFile",LogType::progress,"AudioFile object is created");
}

/**
 * @brief	AudioFile destructor.
 */
File::~File() {
    if(this->exists()) {
        this->file->close();
    }
    consolelog("AudioFile",LogType::progress,"AudioFile object is deleted");
}

/**
 * @brief   It sets the file path name
 * @param   filename        file path name
 */
void File::setFilename(std::string filename) {
    this->filename = filename;
    this->file = new std::fstream(filename, std::ios::in | std::ios::binary | std::ios::ate);
    consolelog("AudioFile",LogType::progress,"file name set to \"" + filename + "\"");
}

/**
 * @brief   It gets the file path name.
 * @return  file path name
 */
std::string File::getFilename() {
    return this->filename;
}

/**
 * @brief   It sets the file reading cursor to keep on reading from another position.
 * @param   cursor          new cursor position [Bytes] from the beginning of the file
 */
void File::setCursor(int cursor) {
    this->cursor = cursor;
    this->file->seekg(cursor, std::ios::beg);
}

/**
 * @brief   It indicates if the file object exists.
 * @return  true if the file object exists
 */
bool File::exists() {
    if(this!=NULL) {
        return this->file->is_open();
    } else {
        return false;
    }
}

/**
 * @brief   It reads data from the file.
 * @param   length          data length in bytes
 * @return  data
 */
std::string File::readData(int length) {
    if(this->exists()) {
        std::string data = "";
        for(int byte = 0; byte < length; byte++) {
            char *datapointer = new char[1];
            file->read(datapointer, 1);
            if(std::string(datapointer)=="") {
                data += '\0';
            } else {
                data += std::string(datapointer);
            }
            delete[] datapointer;
        }
        this->cursor += length;
        return data;
    } else {
        consolelog("AudioFile",LogType::error,"error while reading data from file \"" + this->getFilename() + "\"");
        return NULL;
    }
}

/**
 * @brief   It reads a data number from the file
 * @param   length          data length in bytes
 * @param   endianess       data order (big endian or little endian)
 * @return  data
 */
unsigned long int File::readValue(int length,Endianess::endianess endianess) {
    std::string data = this->readData(length);
    unsigned long int value = 0;
    for (int byte = 0; byte < length; byte++) {
        switch (endianess) {
        case Endianess::bigendian:
        default:
            value |= (unsigned char)data[byte] << (8*(length-byte-1));
            break;
        case Endianess::littleendian:
            value |= (unsigned char)data[byte] << (8*byte);
            break;
        }
    }
    return value;
}

/**
 * @brief	WAVFile constructor.
 */
WAVFile::WAVFile() : File::File() {
    this->cursor = 0;
}

/**
 * @brief	WAVFile constructor.
 * @param   filepath        file path
 */
WAVFile::WAVFile(std::string filename) : File::File(filename) {
    this->readHeader();
}

/**
 * @brief   It reads the file header and sets the format header into the audio file object.
 */
void WAVFile::readHeader() {
    // Format header
    this->header.chunkID = this->readData(4);
    this->header.chunksize = this->readValue(4,Endianess::littleendian);
    this->header.format = this->readData(4);
    // Subchunk 1
    this->header.subchunk1ID = this->readData(4);
    this->header.subchunk1size = this->readValue(4,Endianess::littleendian);
    this->header.audioformat = this->readValue(2,Endianess::littleendian);
    this->header.numchannels = this->readValue(2,Endianess::littleendian);
    this->header.samplerate = this->readValue(4,Endianess::littleendian);
    this->header.byterate = this->readValue(4,Endianess::littleendian);
    this->header.blockalign = this->readValue(2,Endianess::littleendian);
    this->header.bitspersample = this->readValue(2,Endianess::littleendian);
    if(this->header.subchunk1size>16) {
        this->readValue(this->header.subchunk1size-16,Endianess::bigendian);
    }
    // Subchunk 2
    this->header.subchunk2ID = this->readData(4);
    this->header.subchunk2size = this->readValue(4,Endianess::littleendian);
    // Attributes
    this->duration = this->header.subchunk2size / this->header.byterate;
    // Format analyzer
    bool incorrect = false;
    std::string incorrectfields = "";
    if(this->header.chunkID!="RIFF") {
        incorrect = true;
        incorrectfields += "\n\tchunk ID = " + this->header.chunkID;
    }
    if(this->header.chunksize < (20+this->header.subchunk1size + this->header.subchunk2size)) {
        incorrect = true;
        incorrectfields += "\n\tchunk size = " + std::to_string(this->header.chunksize);
    }
    if(this->header.format != "WAVE") {
        incorrect = true;
        incorrectfields += "\n\tformat = " + this->header.format;
    }
    if(this->header.subchunk1ID != "fmt ") {
        incorrect = true;
        incorrectfields += "\n\tsubchunk 1 ID = " + this->header.subchunk1ID;
    }
    if(this->header.subchunk1size < 16) {
        incorrect = true;
        incorrectfields += "\n\tsubchunk 1 size lower than 16 = " + std::to_string(this->header.subchunk1size);
    }
    if(this->header.audioformat != 1) {
        incorrect = true;
        incorrectfields += "\n\taudio format is not PCM (audioformat = " + std::to_string(this->header.audioformat) + ")";
    }
    if(this->header.numchannels < 1) {
        incorrect = true;
        incorrectfields += "\n\tnumber of channels lower than 1 = " + std::to_string(this->header.numchannels);
    }
    if(this->header.samplerate == 0) {
        incorrect = true;
        incorrectfields += "\n\tsample rate = " + std::to_string(this->header.samplerate);
    }
    if(this->header.byterate != (this->header.samplerate*this->header.numchannels*this->header.bitspersample/8)) {
        incorrect = true;
        incorrectfields += "\n\tbyte rate = " + std::to_string(this->header.byterate);
    }
    if(this->header.blockalign != (this->header.numchannels*this->header.bitspersample/8)) {
        incorrect = true;
        incorrectfields += "\n\tblock align = " + std::to_string(this->header.blockalign);
    }
    if(this->header.bitspersample == 0) {
        incorrect = true;
        incorrectfields += "\n\tbits per sample = " + std::to_string(this->header.bitspersample);
    }
    if(this->header.subchunk2ID != "data") {
        incorrect = true;
        incorrectfields += "\n\tsubchunk 2 ID = " + this->header.subchunk2ID;
    }
    if(this->header.subchunk2size == 0) {
        incorrect = true;
        incorrectfields += "\n\tsubchunk 2 size = " + std::to_string(this->header.subchunk2size);
    }
    if (incorrect) {
        consolelog("AudioFile",LogType::warning,"audio file incorret format:" + incorrectfields);
    } else {
        consolelog("AudioFile",LogType::progress,"audio file format correctly loaded");
    }
}
