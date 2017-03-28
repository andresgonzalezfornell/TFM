#ifndef INPUT_H
#define INPUT_H

// System libraries
#include "math.h"
#include "stdlib.h"
// Qt libraries
#include "QAudioInput"
#include "QAudioDeviceInfo"
#include "QByteArray"
#include "QComboBox"
#include "QMainWindow"
#include "QObject"
#include "QPixmap"
#include "QPushButton"
#include "QSlider"
#include "QWidget"
#include "QPainter"
#include "QLayout"
#include "qendian.h"
// Classes and local files
#include "../tools/Logger.h"

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
 * @class	DeviceLevel
 * @author	Andrés González Fornell
 * @brief	Input audio volumeter class.
 */
class DeviceLevel : public QWidget
{
    Q_OBJECT
public:
    DeviceLevel(QWidget *parent);
public slots:
    void setLevel(float level);
protected:
    void paintEvent(QPaintEvent *event);
private:
    int width;          /**< Volumeter width */
    qreal level;        /**< Current level */
    QPixmap pixmap;     /**< pixmap */
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
    // Devices
    void updateDevices(QComboBox &device_selector);
    QAudioDeviceInfo getDevice(int index);
    void setDevice(int index);
    // Level
    void setVolume(int value);
    // Controls
    void unmute();
    void mute();
    void switchMuting();
    void initialize();
public slots:
    void sendData(float);
private:
    QWidget *framework;             /**< user interface framework of device */
    QAudioDeviceInfo deviceinfo;    /**< audio device info object */
    QAudioInput *audioinput;        /**< audio input object */
    QAudioFormat format;            /**< audio format object */
    DeviceLevel *volumeter;         /**< volumeter object */
    float fs;                       /**< sampling frequency [Hz] */
signals:
    void newData(float);
};

#endif // INPUT_H
