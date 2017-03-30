#ifndef INPUT_H
#define INPUT_H

// System libraries
#include "math.h"
#include "stdlib.h"
// Qt libraries
#include "QMainWindow"
#include "QObject"
#include "QWidget"
#include "QComboBox"
#include "QLayout"
#include "QPushButton"
#include "QSlider"
#include "QAudioInput"
#include "QAudioDeviceInfo"
#include "QByteArray"
#include "qendian.h"
// Classes and local files
#include "../tools/Logger.h"
#include "volumeter.h"

/**
 * @class	DeviceChannel
 * @author	Andrés González Fornell
 * @brief	Input audio device class.
 */

class DeviceChannel  : public QIODevice
{
    Q_OBJECT
public:
    DeviceChannel(const QAudioFormat &format, QObject *parent);
    ~DeviceChannel();
    void start();
    void stop();
    void unmute();
    void mute();
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 datalength);
private:
    const QAudioFormat audioformat;     /**< audio format of device channel */
    quint32 amplitude;                  /**< absolute maximum value of the audio signal for the selected audio format*/
    bool muted;                         /**< it is true if the audio channel is muted */
signals:
    void newLevel(float);
    void newData(float);
};

/**
 * @class	Device
 * @author	Andrés González Fornell
 * @brief	User interface class of Device framework.
 */
class Device : public QMainWindow
{
    Q_OBJECT
public:
    DeviceChannel *channel;         /**< device channel */
    Device(QWidget *framework, float fs);
    ~Device();
    void initialize();
    void updateDevices(QComboBox &device_selector);
    QAudioDeviceInfo getDevice(int index);
    void unmute();
    void mute();
public slots:
    void sendData(float data);
    void setDevice(int index);
    void switchMuting();
    void setLevel(int value);
private:
    QWidget *framework;             /**< user interface framework of device */
    QAudioDeviceInfo deviceinfo;    /**< audio device info object */
    QAudioInput *audioinput;        /**< audio input object */
    QAudioFormat format;            /**< audio format object */
    Volumeter *volumeter;           /**< volumeter object */
    float fs;                       /**< sampling frequency [Hz] */
signals:
    void newData(float);
};

#endif // INPUT_H
