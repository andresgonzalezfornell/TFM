#ifndef CHANNELS_H
#define CHANNELS_H

// System libraries
#include "string"
#include "math.h"
#include "stdlib.h"
// Qt libraries
#include "QMainWindow"
#include "QObject"
#include "QFile"
#include "QWidget"
#include "QLayout"
#include "QLabel"
#include "QLineEdit"
#include "QTimeEdit"
#include "QPushButton"
#include "QComboBox"
#include "QSpinBox"
#include "QCheckBox"
#include "QIcon"
#include "QPixmap"
#include "QVector"
// Classes and local files
#include "AudioFile.h"
#include "AudioInfo.h"
#include "AudioSignal.h"
#include "AudioChart.h"
#include "Volumeter.h"
#include "../tools/AudioStream.h"
#include "../tools/Logger.h"

/**
 * @class	Channels
 * @author	Andrés González Fornell
 * @brief	Single-object input configuration class.
 */
class Channel {
public:
    float level;                    /**< current audio volume level */
    AudioFile *file;                /**< source audio file object */
    bool active;                    /**< it indicates if the audio source is active */
    bool paused;                    /**< it indicates if the audio is paused */
    bool preview;                   /**< it indicates if the audio file preview is unmuted */
    /**
     * @brief   user interface elements
     * @{
     */
    QSlider *levelslider;           /**< volume level slider */
    QGroupBox *groupbox;            /**< channel group box */
    QLineEdit *label;               /**< field to change the channel label */
    QWidget *volumeterwidget;       /**< channel level indicator */
    QCheckBox *activecheckbox;      /**< checkbox to indicate if the channel is active */
    QPushButton *loadfile;          /**< button to load file as source */
    QLineEdit *currentsource;       /**< current source indicator */
    QProgressBar *audioprogress;    /**< file playback progress bar */
    QPushButton *playpause;         /**< file playback play/pause button */
    QPushButton *previewbutton;     /**< audio file preview button */
    QPushButton *info;              /**< audio file info button */
    QTimeEdit *audiotime;           /**< file playback current time field */
    /** @} */
    Channel(QLayout *parent, int index);
    ~Channel();
    int getIndex() const;
    void setIndex(int index);
    void setLevel(int level);
    void setLabel(std::string label);
    void setActive(bool state);
    void setFile(std::string filepath);
    void setAudioProgress(float value);
    void playFile();
    void pauseFile();
    void unmutePreview();
    void mutePreview();
    void openInfo();
    void setAudioTime(QTime time);
    void setAudioTime(int milliseconds);
    float readData();
    void sendData(float data);
    float getLastValue();
private:
    int index;                      /**< channel index */
    AudioStream *audiostream;       /**< audio channel stream */
    Volumeter *volumeter;           /**< volumeter object */
    std::string name;               /**< input channel name */
    float lastvalue;                /**< last received value */
};

/**
 * @class	ChannelsConfiguration
 * @author	Andrés González Fornell
 * @brief	Channels configuration class.
 */
class ChannelsConfiguration : public QObject {
    Q_OBJECT
public:
    ChannelsConfiguration(QWidget *parent, int number);
    ~ChannelsConfiguration();
    Channel *getChannel(int index);
    int getNumber();
    void deleteChannel(int index);
    int getChannelIndex(QObject *element);
private:
    QWidget *framework;             /**< user interface framework of channels configuration */
    QList<Channel *> channels;      /**< list of channels */
    QLayout *layout;                /**< user interface layout of the channels list */
    int number;                     /**< number of channels */
    Volumeter *volumeter;           /**< volumeter object */
private slots:
    // Channels configuration controls
    void setNumber(int number);
    // Inputs channels controls
    void setLevel(int level);
    void setLabel(QString label);
    void setActive(bool state);
    void loadFile();
    void switchPlayPause();
    void switchPreview();
    void openInfo();
};

/**
 * @class	Channels
 * @author	Andrés González Fornell
 * @brief	User interface class of Channels framework.
 */
class Channels : public QMainWindow
{
    Q_OBJECT
public:
    static int fs;                                      /**< signal sampling frequency [Hz] */
    ChannelsConfiguration *channelsconfiguration;       /**< channels configuration object */
    AudioChart *audiochart;                             /**< audio chart object */
    Channels(QWidget *framework);
    ~Channels();
    void setfs(int fs);
private:
    AudioSignal *master;                                /**< input master audio signal */
};

#endif // CHANNELS_H
