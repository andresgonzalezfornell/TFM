#ifndef INPUT_H
#define INPUT_H

// Qt libraries
#include <QAudioInput>
#include <QByteArray>
#include <QComboBox>
#include <QMainWindow>
#include <QObject>
#include <QPixmap>
#include <QPushButton>
#include <QSlider>
#include <QWidget>
// Classes and local files
#include "../tools/Logger.h"

class InputChannel  : public QIODevice
{
    Q_OBJECT
public:
    InputChannel(const QAudioFormat &format, QObject *parent);
    ~InputChannel();
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
    void update();
};

class InputLevel : public QWidget
{
    Q_OBJECT
public:
    InputLevel(QWidget *parent);
    void setLevel(qreal value);
protected:
    void paintEvent(QPaintEvent *event);
private:
    int width;
    qreal level;
    QPixmap pixmap;
};

class Input : public QMainWindow
{
    Q_OBJECT
public:
    Input(QWidget *ui_input);
    ~Input();
    // Devices
    void updateDevices(QComboBox &input_device);
    QAudioDeviceInfo getDevice(int index);
    void setDevice(int index);
    // Level
    void setVolume(int value);
    // Ui
    void playPause(QPushButton *button);
    void switchMode(QPushButton *button);
    void initialize();
    
public slots:
    void refresh();
private:
    QWidget *ui_input;
    QAudioDeviceInfo device;
    InputChannel *channel;
    QAudioInput *audioinput;
    QAudioFormat format;
    InputLevel *volumeter;
};

#endif // INPUT_H
