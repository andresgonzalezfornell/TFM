#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <math.h>
#include "QtMath"

// C libraries
#include "stdlib.h"
// Qt libraries
#include "QIODevice"
#include "QDialog"
#include "QtGlobal"
#include "QtEndian"
#include "QList"
#include "QAudioDeviceInfo"
#include "QAudioOutput"
#include "QComboBox"
#include "QDoubleSpinBox"
// Classes and local files
#include "../process/AudioSignal.h"
#include "../tools/Logger.h"

namespace Ui {
class AudioTest;
}

/**
 * @class	OutputDevice
 * @author	Andrés González Fornell
 * @brief	Audio output device class (QIODevice extension).
 */
class OutputDevice : public QIODevice {
    Q_OBJECT
public:
    OutputDevice(QAudioFormat format);
    ~OutputDevice();
    qint64 readData(char *data, qint64 length);
    qint64 writeData(const char *data, qint64 length);
    qint64 bytesAvailable() const;
    qint64 cursor;
    void send(AudioSignal signal);
    void test(double amplitude, double frequency, float duration);
private:
    QAudioFormat format;                /**< audio format object */
    QByteArray buffer;                  /**< audio output data buffer */
    long int amplitude;                 /**< audio signal max amplitude */
    double offset;                      /**< audio signal offset (used only for unsigned format) */
};

/**
 * @class	AudioOutput
 * @author	Andrés González Fornell
 * @brief	Audio output class.
 */
class AudioOutput : public QObject
{
    Q_OBJECT
public:
    OutputDevice *outputdevice;         /**< audio output QIODevice class object to control audio output device functions*/
    int fs;                             /**< signal sampling frequency [Hz] */
    int samplesize;                     /**< audio sample size [bits] */
    float volume;                       /**< audio output volume */
    AudioOutput(QComboBox *selector, int fs, int samplesize);
    ~AudioOutput();
    void start();
    void stop();
    void setDevices();
    void setDevices(QList<QAudioDeviceInfo> devices);
    void setVolume(float volume);
public slots:
    void setDevice(int index);
private:
    QList<QAudioDeviceInfo> devices;    /**< list of system audio output devices */
    QAudioDeviceInfo device;            /**< current system audio output device */
    QAudioOutput *audiooutput;          /**< audio output object */
    QAudioFormat format;                /**< output audio format object */
    QByteArray buffer;                  /**< audio output buffer object */
    QComboBox *selector;                /**< user interface combo box to select audio device */
    void initialize();
};

/**
 * @class	AudioTest
 * @author	Andrés González Fornell
 * @brief	Audio output test class.
 */
class AudioTest : public QDialog {
    Q_OBJECT
public:
    AudioTest(QWidget *parent = 0);
    ~AudioTest();
private:
    Ui::AudioTest *ui;                  /**< user interface object */
    AudioOutput *audiooutput;            /**< audio output object */
    int getSampleSize();
    int getFS();
    void start();
    void play();
private slots:
    void setDevice(int index);
    void setSampleFormat(int index);
    void setFS(int index);
    void setFrequency(double frequency);
    void setAmplitude(int amplitude);
};

#endif // AUDIOOUTPUT_H
