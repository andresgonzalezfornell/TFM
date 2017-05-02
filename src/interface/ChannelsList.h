#ifndef CHANNELSLIST_H
#define CHANNELSLIST_H

// System libraries
#include "cmath"
#include "stdlib.h"
// Qt libraries
#include "QObject"
#include "QWidget"
#include "QLayout"
#include "QLabel"
#include "QCheckBox"
// Classes and local files
#include "AudioOutput.h"
#include "AudioChart.h"
#include "Volumeter.h"
#include "../process/AudioSignal.h"
#include "../process/AudioStream.h"
#include "../tools/Logger.h"

/**
 * @class   Channel
 * @author  Andrés González Fornell
 * @brief   Single-object class from channels list.
 */
class Channel {
public:
    float volume;                   /**< current audio volume level */
    AudioOutput *audiooutput;       /**< system audio output devices object */
    AudioChart *chart;              /**< audio chart object */
    /**
     * @brief   user interface elements
     * @{
     */
    QGroupBox *groupbox;            /**< channel group box */
    QLineEdit *label;               /**< field to change the channel label */
    QSlider *volumeslider;          /**< volume level slider */
    QCheckBox *mutecheckbox;        /**< muted checkbox object */
    QWidget *volumeterwidget;       /**< volumeter framework */
    QCheckBox *bypasscheckbox;      /**< checkbox object to bypass effect */
    QComboBox *deviceselector;      /**< audio output device selector object */
    QWidget *chartwidget;           /**< audio chart widget object */
    /** @} */
    Channel(QLayout *framework, std::string prefix, int index, bool isoutput);
    ~Channel();
    int getIndex();
    void setIndex(int index);
    void setLabel(std::string label);
    void setVolume(int volume);
    void mute(bool state);
    void bypass(bool state);
private:
    int index;                      /**< channel index */
    bool muted;                     /**< it indicates if channel is muted */
    bool bypassed;                  /**< it tells channel to bypass effects or apply them */
    AudioStream *stream;            /**< channel audio stream object */
    std::string prefix;             /**< user interface prefix */
    std::string name;               /**< channel name */
    bool isoutput;                  /**< true to show audio output device selector (in case of sending channel to speakers or other audio output system device) */
    Volumeter *volumeter;           /**< volumeter object */
};

/**
 * @class   ChannelsList
 * @author  Andrés González Fornell
 * @brief   Channels list class. It shows information about channels signals.
 */
class ChannelsList : public QObject {
    Q_OBJECT
public:
    static int fs;                  /**< signal sampling frequency */
    static int samplesize;          /**< signal sample size [bits] */
    ChannelsList(QWidget *framework, int number, bool showdevices);
    ~ChannelsList();
    Channel *getChannel(int index);
    void deleteChannel(int index);
    void setChannelsNumber(int number);
private:
    QList<Channel *> channels;      /**< list of channels */
    std::string prefix;             /**< user interface prefix */
    bool showdevices;               /**< true to show audio output device selector (in case of sending channels to speakers or other audio output system devices) */
    QWidget *framework;             /**< user interface framework of channels list */
    QLayout *layout;                /**< user interface layout of channels list */
    Volumeter *volumeter;           /**< volumeter object */
    int getChannelIndex(QObject *element);
private slots:
    void setLabel(QString label);
    void setVolume(int volume);
    void mute(bool state);
    void bypass(bool state);
    void setDevice(int device);
};

#endif // CHANNELSLIST_H
