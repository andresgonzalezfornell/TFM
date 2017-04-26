#include "File.h"

/**
 * @brief	File constructor.
 */
File::File(bool writepermission) {
    this->cursor = 0;
    this->writepermission = writepermission;
    consolelog("File",LogType::progress,"File object is created");
}

/**
 * @brief	File constructor.
 * @param   filepath        file path
 */
File::File(std::string filename, bool writepermission) : File::File(writepermission) {
    this->setFilename(filename);
    this->setCursor(0);
}

/**
 * @brief	File destructor.
 */
File::~File() {
    if(this->exists()) {
        this->file->close();
    }
    consolelog("File",LogType::progress,"File object is deleted");
}

/**
 * @brief   It sets the file path name
 * @param   filename        file path name
 */
void File::setFilename(std::string filename) {
    this->filename = filename;
    if (this->writepermission) {
        this->file = new std::fstream(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    } else {
        this->file = new std::fstream(filename, std::ios::binary | std::ios::in);
    }
    consolelog("File",LogType::progress,"file name set to \"" + filename + "\"");
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
 * @brief   It gets the current file reading cursor.
 * @return  cursor [Bytes] from the beginning of the file
 */
int File::getCursor() {
    return this->cursor;
}

/**
 * @brief   It gets the total file size.
 * @return  file size [Bytes]
 */
int File::size() {
    struct stat::stat fileinfo;
    if (stat(this->filename.c_str(), &fileinfo) == 0) {
        return fileinfo.st_size;
    } else {
        consolelog("File",LogType::error,"error while getting file size");
        return 0;
    }
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
 * @brief   It reads binary from the file.
 * @param   length          data length [Bytes]
 * @return  data
 */
char *File::read(int length) {
    if(this->exists()) {
        if(this->cursor + length <= this->size()) {
            char *data = new char[length];
            this->file->read(data, length);
            this->cursor += length;
            return data;
        } else {
            consolelog("File",LogType::error, "file \"" + this->getFilename() + "\" does not contain more data");
            return NULL;
        }
    } else {
        consolelog("File",LogType::error,"error while reading data from file \"" + this->getFilename() + "\"");
        return NULL;
    }
}

/**
 * @brief   It reads data from the file.
 * @param   length          data length [Bytes]
 * @return  data
 */
std::string File::readData(int length) {
    std::string data = "";
    char *datapointer = this->read(length);
    if (datapointer != NULL) {
        for(int byte = 0; byte < length; byte++) {
            data += *(datapointer+byte);
        }
        return data;
    } else {
        return NULL;
    }
}

/**
 * @brief   It writes data on the file.
 * @param   data
 */
void File::writeData(std::string data) {
    if (this->writepermission) {
        const char *datapointer = data.c_str();
        this->file->write(datapointer, data.size());
        this->cursor += data.length();
    } else {
        consolelog("File",LogType::error,"file has not write permission");
    }
}

/**
 * @brief   It reads a data number from the file
 * @param   length          data length [Bytes]
 * @param   endianess       data order (big endian or little endian)
 * @return  data
 */
unsigned int File::readNumber(int length,Endianess::endianess endianess) {
    char *data = this->read(length);
    unsigned int value = 0;
    for (int byte = 0; byte < length; byte++) {
        switch (endianess) {
        case Endianess::bigendian:
        default:
            value |= (unsigned char)*(data+byte) << (8*(length-byte-1));
            break;
        case Endianess::littleendian:
            value |= (unsigned char)*(data+byte) << (8*byte);
            break;
        }
    }
    return value;
}

/**
 * @brief   It writes a data number on the file
 * @param   value
 * @param   length          data length [Bytes]
 * @param   endianess       data order (big endian or little endian)
 */
void File::writeNumber(unsigned int value, int length, Endianess::endianess endianess) {
    std::string data = "";
    char number = 0;
    for (int byte = 0; byte<length; byte++) {
        switch (endianess) {
        case Endianess::bigendian:
        default:
            number = value >> (8*(length-byte-1));
            break;
        case Endianess::littleendian:
            number = value >> (8*byte);
            break;
        }
        data += std::string(1,number);
    }
    this->writeData(data);
}

/**
 * @brief	WAVFile constructor.
 * @param   writepermission file write permission (true if it is allowed)
 */
WAVFile::WAVFile(bool writepermission) : File::File(writepermission) {
}

/**
 * @brief	WAVFile constructor.
 * @param   filepath        file path
 * @param   writepermission file write permission (true if it is allowed)
 */
WAVFile::WAVFile(std::string filename, bool writepermission) : File::File(filename, writepermission) {
    this->readHeader();
}

/**
 * @brief   WAVFile constructor. Write file is allowed.
 * @param   filename        file path
 * @param   channels        number of channels
 * @param   samplerate      sample rate
 * @param   sampleformat    number of bits of a sample
 */
WAVFile::WAVFile(std::string filename, int channels, int fs, int sampleformat) : File::File(filename,true) {
    this->header.chunkID = "RIFF";
    this->header.chunksize = 36;
    this->header.format = "WAVE";
    this->header.subchunk1ID = "fmt ";
    this->header.subchunk1size = 16;
    this->header.audioformat = 1;
    this->header.numchannels = channels;
    this->header.samplerate = fs;
    this->header.byterate = (int)fs*channels*sampleformat/8;
    this->header.blockalign = (int)channels*sampleformat/8;
    this->header.bitspersample = sampleformat;
    this->header.subchunk2ID = "data";
    this->header.subchunk2size = 0;
    this->writeHeader();
}

/**
 * @brief   It sets the signal reading cursor to keep on reading from another position.
 * @param   cursor          new cursor position in samples (instead of bytes) from the beginning of the signal (instead of the file)
 */
void WAVFile::setCursor(int cursor) {
    File::setCursor((int)cursor*this->header.bitspersample/8 + this->header.size());
}

/**
 * @brief   It gets the current signal reading cursor.
 * @return  cursor [Bytes] from the beginning of the signal (instead of the file)
 */
int WAVFile::getCursor() {
    return (int)(File::getCursor() - this->header.size())/(this->header.bitspersample/8);
}

/**
 * @brief   It gets the number of audio samples.
 * @return  number of audio samples
 */
int WAVFile::samples() {
    return this->header.subchunk2size / (this->header.bitspersample / 8);
}

/**
 * @brief   It reads the file header and sets the format header into the audio file object.
 */
void WAVFile::readHeader() {
    // Format header
    this->header.chunkID = this->readData(4);
    this->header.chunksize = this->readNumber(4,Endianess::littleendian);
    this->header.format = this->readData(4);
    // Subchunk 1
    this->header.subchunk1ID = this->readData(4);
    this->header.subchunk1size = this->readNumber(4,Endianess::littleendian);
    this->header.audioformat = this->readNumber(2,Endianess::littleendian);
    this->header.numchannels = this->readNumber(2,Endianess::littleendian);
    this->header.samplerate = this->readNumber(4,Endianess::littleendian);
    this->header.byterate = this->readNumber(4,Endianess::littleendian);
    this->header.blockalign = this->readNumber(2,Endianess::littleendian);
    this->header.bitspersample = this->readNumber(2,Endianess::littleendian);
    File::setCursor(File::getCursor()+this->header.subchunk1size-16);
    // Subchunk 2
    this->header.subchunk2ID = this->readData(4);
    this->header.subchunk2size = this->readNumber(4,Endianess::littleendian);
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
        consolelog("File",LogType::warning,"audio file incorret format:" + incorrectfields);
    } else {
        consolelog("File",LogType::progress,"audio file format correctly loaded");
    }
}

/**
 * @brief   It writes the header on the file from the audio file object header.
 */
void WAVFile::writeHeader() {
    int cursor = File::getCursor();
    File::setCursor(0);
    this->writeData(this->header.chunkID);
    this->writeNumber(this->header.chunksize,4,Endianess::littleendian);
    this->writeData(this->header.format);
    this->writeData(this->header.subchunk1ID);
    this->writeNumber(this->header.subchunk1size,4,Endianess::littleendian);
    this->writeNumber(this->header.audioformat,2,Endianess::littleendian);
    this->writeNumber(this->header.numchannels,2,Endianess::littleendian);
    this->writeNumber(this->header.samplerate,4,Endianess::littleendian);
    this->writeNumber(this->header.byterate,4,Endianess::littleendian);
    this->writeNumber(this->header.blockalign,2,Endianess::littleendian);
    this->writeNumber(this->header.bitspersample,2,Endianess::littleendian);
    this->writeData(this->header.subchunk2ID);
    this->writeNumber(this->header.subchunk2size,4,Endianess::littleendian);
    if (cursor > this->header.size()) {
        File::setCursor(cursor);
    }
    consolelog("File", LogType::progress, "audio file header was written on the file");
}

/**
 * @brief   It reads a sample from the audio file.
 * @return  sample value (from -1 to 1)
 */
float WAVFile::readValue() {
    int amplitude = 0x1 << (this->header.bitspersample - 1);
    int value = this->readNumber(this->header.bitspersample/8,Endianess::littleendian);
    unsigned int signmask = amplitude;
    unsigned int valuemask = signmask - 1;
    bool negative = (value & signmask) != 0;
    if(negative) {
        value = (~value) & valuemask;   // inverting bits
        value += 1;                     // 2's complement
        value *= -1;                    // sign
    }
    return (float)value / amplitude;
}

/**
 * @brief   It writes a sample on the audio file.
 * @param   value           sample value (from -1 to 1)
 */
void WAVFile::writeValue(float value) {
    int amplitude = 0x1 << (this->header.bitspersample - 1);
    unsigned int data = value*amplitude;
    // Solving value according to preccision
    if(data == amplitude && value > 0) {
        data--;
    }
    this->writeNumber(data, (int)this->header.bitspersample/8, Endianess::littleendian);
    // Updating audio file header
    int cursor = this->getCursor();
    if (cursor > 0) {
        this->header.chunksize = this->size()-8;
        this->header.subchunk2size = this->header.chunksize - this->header.size();
    }
}
