#ifndef CHANNELSLIST_H
#define CHANNELSLIST_H

// System libraries
#include "string"
#include "math.h"
#include "stdlib.h"
// Qt libraries
#include "QObject"
#include "QFile"
#include "QWidget"
#include "QLayout"
#include "QLabel"
#include "QCheckBox"
// Classes and local files
#include "AudioSignal.h"
#include "AudioChart.h"
#include "Volumeter.h"
#include "../tools/AudioStream.h"
#include "../tools/Logger.h"

/**
 * @class   Channel
 * @author  Andrés González Fornell
 * @brief   Single-object class from channels list.
 */
class Channel {
public:
    float level;                    /**< current audio volume level */
    /**
     * @brief   user interface elements
     * @{
     */
    QGroupBox *groupbox;            /**< channel group box */
    QLineEdit *label;               /**< field to change the channel label */
    QSlider *levelslider;           /**< volume level slider */
    QCheckBox *mutecheckbox;        /**< muted checkbox object */
    QWidget *volumeterwidget;       /**< volumeter framework */
    QCheckBox *bypasscheckbox;      /**< checkbox object to bypass effect*/
    AudioChart *chart;              /**< audio chart object */
    /** @} */
    Channel(QLayout *framework, std::string prefix, int index);
    ~Channel();
    int getIndex();
    void setIndex(int index);
    void setStream(AudioStream *stream);
    void setLabel(std::string label);
    void setLevel(int level);
    void mute(bool state);
    void bypass(bool state);
private:
    int index;                      /**< channel index */
    bool muted;                     /**< it indicates if channel is muted */
    bool bypassed;                  /**< it tells channel to bypass effects or apply them */
    AudioStream *stream;            /**< channel audio stream object */
    std::string prefix;             /**< user interface prefix */
    std::string name;               /**< channel name */
    Volumeter *volumeter;           /**< volumeter object */
};

/**
 * @class   ChannelsList
 * @author  Andrés González Fornell
 * @brief   Channels list class. It shows information about channels signals.
 */
class ChannelsList : public QObject
{
    Q_OBJECT
public:
    static int fs;                  /**< signal sampling frequency */
    ChannelsList(QWidget *framework, int number);
    ~ChannelsList();
    Channel *getChannel(int index);
    void deleteChannel(int index);
    void setChannelsNumber(int number);
private:
    QList<Channel *> channels;      /**< list of channels */
    std::string prefix;             /**< user interface prefix */
    QWidget *framework;             /**< user interface framework of channels list */
    QLayout *layout;                /**< user interface layout of channels list */
    Volumeter *volumeter;           /**< volumeter object */
    QList<Channel *> list;
    int getChannelIndex(QObject *element);
private slots:
    void setLabel(QString label);
    void setLevel(int level);
    void mute(bool state);
    void bypass(bool state);
};

#endif // CHANNELSLIST_H
