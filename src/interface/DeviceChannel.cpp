// Class libraries
#include "device.h"
#include "ui_mainwindow.h"

/**
 * @brief	DeviceChannel constructor.
 */
DeviceChannel::DeviceChannel(const QAudioFormat &format, QObject *parent)
    :   QIODevice(parent)
    ,   audioformat(format)
    ,   amplitude(0)
{
    int sign;
    std::string audioformat_description = std::to_string(audioformat.sampleSize()) + "bits ";
    switch(audioformat.sampleType()) {
    case QAudioFormat::UnSignedInt:
        sign = 0;
        audioformat_description += "unsigned int";
        break;
    case QAudioFormat::SignedInt:
        audioformat_description += "signed int";
        sign = 1;
        break;
    case QAudioFormat::Float:
        audioformat_description += "float";
        sign = 1;
    default:
        sign = 0;
        break;
    }
    amplitude = pow(2,audioformat.sampleSize()-sign);
    consolelog("Device",LogType::info,"Audio format: " + audioformat_description);
    consolelog("Device",LogType::progress,"DeviceChannel object is created");
}

/**
 * @brief	DeviceChannel destructor.
 */
DeviceChannel::~DeviceChannel()
{
    consolelog("Device",LogType::progress,"DeviceChannel object is deleted");
}

/**
 * @brief	It starts device channel.
 */
void DeviceChannel::start()
{
    open(QIODevice::WriteOnly);
    consolelog("Device",LogType::progress,"DeviceChannel has started");
}

/**
 * @brief	It stops device channel.
 */
void DeviceChannel::stop()
{
    close();
    consolelog("Device",LogType::progress,"DeviceChannel has stopped");
}

/**
 * @brief	It unmutes device channel.
 */
void DeviceChannel::unmute()
{
    this->muted = false;
    consolelog("Device",LogType::progress,"DeviceChannel has been unmuted");
}

/**
 * @brief	It mutes device channel.
 */
void DeviceChannel::mute()
{
    this->muted = true;
    consolelog("Device",LogType::progress,"DeviceChannel has been muted");
}

/**
 * @brief	It reads data from the device channel.
 * @param   *data       data pointer
 * @param   maxlen      maximum lenght
 * @return
 */
qint64 DeviceChannel::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data)
    Q_UNUSED(maxlen)
    return 0;
}

/**
 * @brief	It writes data from the device channel.
 * @param   *data       data pointer
 * @param   datalength  data length
 * @return
 */
qint64 DeviceChannel::writeData(const char *data, qint64 datalength)
{
    if (this->amplitude) {
        const int channels = audioformat.channelCount();
        const int channelBytes = audioformat.sampleSize() / 8;
        const int sampleBytes = channels * channelBytes;
        const int samples = datalength / sampleBytes;
        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);
        float value;
        for (int sample = 0; sample < samples; ++sample) {
            for (int channel = 0; channel < channels; channel++) {
                if (this->muted) {
                    value = 0;
                } else {
                    switch(this->audioformat.sampleSize()) {
                    case 8:
                        switch(this->audioformat.sampleType()) {
                        case QAudioFormat::UnSignedInt:
                            value = QVariant(*reinterpret_cast<const quint8*>(ptr)).toFloat();
                            break;
                        case QAudioFormat::SignedInt:
                            value = QVariant(*reinterpret_cast<const qint8*>(ptr)).toFloat();
                            break;
                        }
                        break;
                    case 16:
                        switch(this->audioformat.sampleType()) {
                        case QAudioFormat::UnSignedInt:
                            switch (this->audioformat.byteOrder()) {
                            case QAudioFormat::LittleEndian:
                                value = QVariant(qFromLittleEndian<quint16>(ptr)).toFloat();
                                break;
                            case QAudioFormat::BigEndian:
                                value = QVariant(qFromBigEndian<quint16>(ptr)).toFloat();
                                break;
                            }
                            break;
                        case QAudioFormat::SignedInt:
                            switch (this->audioformat.byteOrder()) {
                            case QAudioFormat::LittleEndian:
                                value = QVariant(qFromLittleEndian<qint16>(ptr)).toFloat();
                                break;
                            case QAudioFormat::BigEndian:
                                value = QVariant(qFromBigEndian<qint16>(ptr)).toFloat();
                                break;
                            }
                            break;
                        }
                        break;
                    case 32:
                        switch(this->audioformat.sampleType()) {
                        case QAudioFormat::UnSignedInt:
                            switch (this->audioformat.byteOrder()) {
                            case QAudioFormat::LittleEndian:
                                value = QVariant(qFromLittleEndian<quint32>(ptr)).toFloat();
                                break;
                            case QAudioFormat::BigEndian:
                                value = QVariant(qFromBigEndian<quint32>(ptr)).toFloat();
                                break;
                            }
                            break;
                        case QAudioFormat::SignedInt:
                            switch (this->audioformat.byteOrder()) {
                            case QAudioFormat::LittleEndian:
                                value = QVariant(qFromLittleEndian<qint32>(ptr)).toFloat();
                                break;
                            case QAudioFormat::BigEndian:
                                value = QVariant(qFromBigEndian<qint32>(ptr)).toFloat();
                                break;
                            }
                            break;
                        case QAudioFormat::Float:
                            value = *reinterpret_cast<const float*>(ptr);
                            break;
                        }
                        break;
                    }
                }
                value /= this->amplitude;   // normalization
                emit newData(value);
                ptr += channelBytes;
            }
        }
    }
    return datalength;
}
