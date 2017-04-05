#include "AudioFile.h"

/**
 * @brief	Objects constructor.
 */
AudioFile::AudioFile() {
    this->file = new QFile();
    this->file->open(QIODevice::ReadOnly);
    this->cursor = 0;
    consolelog("AudioFile",LogType::progress,"AudioFile object is created");
}

/**
 * @brief	Objects constructor.
 * @param   filepath        file path
 */
AudioFile::AudioFile(std::string filepath) {
    this->file = new QFile(QString::fromStdString(filepath));
    this->file->open(QIODevice::ReadOnly);
    this->cursor = 0;
    this->readHeader();
    consolelog("AudioFile",LogType::progress,"AudioFile object is created");
}

/**
 * @brief	Objects destructor.
 */
AudioFile::~AudioFile() {
    consolelog("AudioFile",LogType::progress,"AudioFile object is deleted");
}

/**
 * @brief   It gets the file path.
 * @return  filepath
 */
std::string AudioFile::getFilepath() {
    return this->file->fileName().toStdString();
}

/**
 * @brief   It reads the file header and sets the format header into the audio file object.
 */
void AudioFile::readHeader() {
    // Format header
    this->header.chunkID = QString(this->readData(4)).toStdString();
    this->header.chunksize = this->readDataNumber(4,Endianess::littleendian);
    this->header.format = QString(this->readData(4)).toStdString();
    this->header.subchunk1ID = QString(this->readData(4)).toStdString();
    this->header.subchunk1size = this->readDataNumber(4,Endianess::littleendian);
    this->header.audioformat = this->readDataNumber(2,Endianess::littleendian);
    this->header.numchannels = this->readDataNumber(2,Endianess::littleendian);
    this->header.samplerate = this->readDataNumber(4,Endianess::littleendian);
    this->header.byterate = this->readDataNumber(4,Endianess::littleendian);
    this->header.blockalign = this->readDataNumber(2,Endianess::littleendian);
    this->header.bitspersample = this->readDataNumber(2,Endianess::littleendian);
    this->header.subchunk2ID = QString(this->readData(4)).toStdString();
    this->header.subchunk2size = this->readDataNumber(4,Endianess::littleendian);
    // Attributes
    this->duration = this->header.subchunk2size / this->header.byterate;
    // Format analyzer
    bool incorrect = false;
    std::string incorrectfields = "";
    if(this->header.chunkID!="RIFF") {
        incorrect = true;
        incorrectfields += "\n\tchunk ID = " + this->header.chunkID;
    }
    if(this->header.chunksize != (20+this->header.subchunk1size + this->header.subchunk2size)) {
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
    if(this->header.subchunk1size != 16) {
        incorrect = true;
        incorrectfields += "\n\tsubchunk 1 size = " + std::to_string(this->header.subchunk1size);
    }
    if(this->header.audioformat != 1) {
        incorrect = true;
        incorrectfields += "\n\taudio format = " + std::to_string(this->header.audioformat);
    }
    if(this->header.numchannels != 1) {
        incorrect = true;
        incorrectfields += "\n\tnumber of channels: not equal to 1 (mono) = " + std::to_string(this->header.numchannels);
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
        consolelog("AudioFile",LogType::error,"audio file incorret format:" + incorrectfields);
        QMessageBox dialog;
        dialog.setText(QString::fromStdString("The selected audio file has a format that is not supported. Followings header parameters are not accepted:" + incorrectfields));
        dialog.setIcon(QMessageBox::Critical);
        dialog.setDefaultButton(QMessageBox::Ok);
        switch(dialog.exec()) {
        case QMessageBox::Ok:
            dialog.close();
            break;
        }
    } else {
        consolelog("AudioFile",LogType::progress,"audio file format loaded");
    }
}

/**
 * @brief   It reads data from the file.
 * @param   length          data length in bytes
 * @return  data
 */
QByteArray AudioFile::readData(int length) {
    if(this->file->exists()) {
        if (!this->file->isOpen()) {
            this->file->open(QIODevice::ReadOnly);
        }
        QByteArray data = QByteArray(length,0x00);
        char *datapointer = new char;
        this->file->read(datapointer, length);
        for (int byte = 0; byte < length; byte++) {
            data[byte] = (quint8)*(datapointer+byte);
        }
        this->cursor += length;
        return data;
    } else {
        consolelog("AudioFile",LogType::error,"error while reading data from file \"" + this->getFilepath() + "\"");
        return NULL;
    }
}

/**
 * @brief   It reads a data number from the file
 * @param   length          data length in bytes
 * @param   endianess       data order (big endian or little endian)
 * @return  data
 */
unsigned long int AudioFile::readDataNumber(int length,Endianess::endianess endianess) {
    QByteArray data = this->readData(length);
    if (data.isNull()) {
        return 0;
    } else {
        unsigned long int number = 0;
        for (int byte = 0; byte < length; byte++) {
            switch (endianess) {
            case Endianess::bigendian:
            default:
                number |= (quint8)data[byte] << (8*(length-byte-1));
                break;
            case Endianess::littleendian:
                number |= (quint8)data[byte] << (8*byte);
                break;
            }
        }
        return number;
    }
}

/**
 * @brief   It indicates if the file object exists.
 * @return  true if the file object exists
 */
bool AudioFile::exists() {
    if(this!=NULL) {
        return this->file->exists();
    } else {
        return false;
    }
}
