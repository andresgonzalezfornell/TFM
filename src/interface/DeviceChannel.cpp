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
    string audioformat_description = std::to_string(audioformat.sampleSize()) + "bits ";
    switch(audioformat.sampleType()) {
    case QAudioFormat::UnSignedInt:
        sign = 0;
        audioformat_description += "unsigned";
        break;
    case QAudioFormat::SignedInt:
        audioformat_description += "signed";
        sign = 1;
        break;
    default:
        sign = 0;
        break;
    }
    amplitude = pow(2,audioformat.sampleSize()-sign)-1;
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
 * @param   len         data length
 * @return
 */
qint64 DeviceChannel::writeData(const char *data, qint64 datalength)
{
    if (this->amplitude) {
        Q_ASSERT(audioformat.sampleSize() % 8 == 0);
        const int channelBytes = audioformat.sampleSize() / 8;
        const int sampleBytes = audioformat.channelCount() * channelBytes;
        Q_ASSERT(datalength % sampleBytes == 0);
        const int samples = datalength / sampleBytes;

        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);
        float value, energy;
        int sign;
        quint8 value_uint8 = 0;
        quint16 value_uint16 = 0;
        quint32 value_uint32 = 0;
        for (int sample = 0; sample < samples; ++sample) {
            energy = 0;
//            for (int j = 0; j < audioformat.channelCount(); ++j) {
            switch(audioformat.sampleSize()) {
            case 8:
                value_uint8 = *reinterpret_cast<const quint8*>(ptr);
                switch(audioformat.sampleType()) {
                case QAudioFormat::UnSignedInt:
                    value = QVariant(value_uint8).toFloat();
                    break;
                case QAudioFormat::SignedInt:
                    if((value_uint8 & 0x80)==0) {
                        sign = 1;
                    } else {
                        sign = -1;
                    }
                    value = sign * QVariant(value_uint8 & amplitude).toFloat();
                    break;
                }
                break;
            case 16:
                switch (audioformat.byteOrder()) {
                case QAudioFormat::LittleEndian:
                    value_uint16 = qFromLittleEndian<quint16>(ptr);
                    break;
                case QAudioFormat::BigEndian:
                    value_uint16 = qFromBigEndian<quint16>(ptr);
                    break;
                }
                switch(audioformat.sampleType()) {
                case QAudioFormat::UnSignedInt:
                    value = QVariant(value_uint16).toFloat();
                    break;
                case QAudioFormat::SignedInt:
                    if((value_uint16 & 0x8000)==0) {
                        sign = 1;
                    } else {
                        sign = -1;
                    }
                    value = sign * QVariant(value_uint16 & amplitude).toFloat();
                    break;
                }
                break;
            case 32:
                switch (audioformat.byteOrder()) {
                case QAudioFormat::LittleEndian:
                    value_uint32 = qFromLittleEndian<quint32>(ptr);
                    break;
                case QAudioFormat::BigEndian:
                    value_uint32 = qFromBigEndian<quint32>(ptr);
                    break;
                }
                switch(audioformat.sampleType()) {
                case QAudioFormat::UnSignedInt:
                    value = QVariant(value_uint32).toFloat();
                    break;
                case QAudioFormat::SignedInt:
                    if((value_uint32 & 0x80000000)==0) {
                        sign = 1;
                    } else {
                        sign = -1;
                    }
                    value = sign * QVariant(value_uint32 & amplitude).toFloat();
                    break;
                case QAudioFormat::Float:
                    value = *reinterpret_cast<const float*>(ptr);
                    break;
                }
                break;
            }
            value /= this->amplitude;
            emit newData(value);
            energy = qMax((float)pow(value,2), energy);
            ptr += channelBytes;
//            }
        }
        emit newLevel(energy);
    }
    return datalength;
}
