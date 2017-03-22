/**
 * @name	DeviceChannel.cpp
 * @author	Andrés González Fornell
 * @brief	Input audio device class.
 */

// Class libraries
#include "device.h"
#include "ui_mainwindow.h"
// System libraries
#include "math.h"
#include "stdlib.h"

/**
 * @brief	DeviceChannel constructor.
 */
DeviceChannel::DeviceChannel(const QAudioFormat &format, QObject *parent)
    :   QIODevice(parent)
    ,   audioformat(format)
    ,   amplitude(0)
    ,   level(0.0)
{
    switch (audioformat.sampleSize()) {
    case 8:
        switch (audioformat.sampleType()) {
        case QAudioFormat::UnSignedInt:
            amplitude = 255;
            break;
        case QAudioFormat::SignedInt:
            amplitude = 127;
            break;
        default:
            break;
        }
        break;
    case 16:
        switch (audioformat.sampleType()) {
        case QAudioFormat::UnSignedInt:
            amplitude = 65535;
            break;
        case QAudioFormat::SignedInt:
            amplitude = 32767;
            break;
        default:
            break;
        }
        break;
    case 32:
        switch (audioformat.sampleType()) {
        case QAudioFormat::UnSignedInt:
            amplitude = 0xffffffff;
            break;
        case QAudioFormat::SignedInt:
            amplitude = 0x7fffffff;
            break;
        case QAudioFormat::Float:
            amplitude = 0x7fffffff;
        default:
            break;
        }
        break;

    default:
        break;
    }
    consolelog("Device",progress,"DeviceChannel object is created");
}

/**
 * @brief	DeviceChannel destructor.
 */
DeviceChannel::~DeviceChannel()
{
    consolelog("Device",progress,"DeviceChannel object is deleted");
}

/**
 * @brief	It starts device channel.
 */
void DeviceChannel::start()
{
    open(QIODevice::WriteOnly);
    consolelog("Device",progress,"DeviceChannel has started");
}

/**
 * @brief	It stops device channel.
 */
void DeviceChannel::stop()
{
    close();
    consolelog("Device",progress,"DeviceChannel has stopped");
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
qint64 DeviceChannel::writeData(const char *data, qint64 len)
{
    if (amplitude) {
        Q_ASSERT(audioformat.sampleSize() % 8 == 0);
        const int channelBytes = audioformat.sampleSize() / 8;
        const int sampleBytes = audioformat.channelCount() * channelBytes;
        Q_ASSERT(len % sampleBytes == 0);
        const int numSamples = len / sampleBytes;

        quint32 value_max = 0;
        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

        for (int i = 0; i < numSamples; ++i) {
//            for (int j = 0; j < audioformat.channelCount(); ++j) {
                quint32 value = 0;
                if (audioformat.sampleSize() == 8 && audioformat.sampleType() == QAudioFormat::UnSignedInt) {
                    value = *reinterpret_cast<const quint8*>(ptr);
                } else if (audioformat.sampleSize() == 8 && audioformat.sampleType() == QAudioFormat::SignedInt) {
                    value = qAbs(*reinterpret_cast<const qint8*>(ptr));
                } else if (audioformat.sampleSize() == 16 && audioformat.sampleType() == QAudioFormat::UnSignedInt) {
                    if (audioformat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint16>(ptr);
                    else
                        value = qFromBigEndian<quint16>(ptr);
                } else if (audioformat.sampleSize() == 16 && audioformat.sampleType() == QAudioFormat::SignedInt) {
                    if (audioformat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint16>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint16>(ptr));
                } else if (audioformat.sampleSize() == 32 && audioformat.sampleType() == QAudioFormat::UnSignedInt) {
                    if (audioformat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint32>(ptr);
                    else
                        value = qFromBigEndian<quint32>(ptr);
                } else if (audioformat.sampleSize() == 32 && audioformat.sampleType() == QAudioFormat::SignedInt) {
                    if (audioformat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint32>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint32>(ptr));
                } else if (audioformat.sampleSize() == 32 && audioformat.sampleType() == QAudioFormat::Float) {
                    value = qAbs(*reinterpret_cast<const float*>(ptr) * 0x7fffffff); // assumes 0-1.0
                }
                emit newData(value);
                value_max = qMax(value, value_max);
                ptr += channelBytes;
//            }
        }
        value_max = qMin(value_max, amplitude);
        level = qreal(value_max) / amplitude;
    }
    emit newLevel();
    return len;
}

/**
 * @brief	It gets the current signal level.
 * @return  level
 */
qreal DeviceChannel::getLevel()
{
    return level;
}
