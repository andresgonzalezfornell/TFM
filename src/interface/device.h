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
#include "QVBoxLayout"
#include "qendian.h"
// Classes and local files
#include "../tools/Logger.h"

class DeviceChannel  : public QIODevice
{
    Q_OBJECT
public:
    DeviceChannel(const QAudioFormat &format, QObject *parent);
    ~DeviceChannel();
    void start();
    void stop();
    qreal getLevel();
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
private:
    const QAudioFormat audioformat;
    quint32 amplitude;
    qreal level; // 0.0 <= m_level <= 1.0
signals:
    void newLevel();
    void newData(quint32);
};

class DeviceLevel : public QWidget
{
    Q_OBJECT
public:
    DeviceLevel(QWidget *parent);
    void setLevel(qreal value);
protected:
    void paintEvent(QPaintEvent *event);
private:
    int width;
    qreal level;
    QPixmap pixmap;
};

class Device : public QMainWindow
{
    Q_OBJECT
public:
    Device(QWidget *ui_device);
    ~Device();
    DeviceChannel *channel;
    // Devices
    void updateDevices(QComboBox &device_selector);
    QAudioDeviceInfo getDevice(int index);
    void setDevice(int index);
    // Level
    void setVolume(int value);
    // Ui
    void playPause(QPushButton *button);
    void switchMode(QPushButton *button);
    void initialize();
public slots:
    void updateLevel();
private:
    QWidget *ui_device;
    QAudioDeviceInfo deviceinfo;
    QAudioInput *audioinput;
    QAudioFormat format;
    DeviceLevel *volumeter;
};

#endif // INPUT_H
