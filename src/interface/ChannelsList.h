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
    static int fs;                  /**< signal sampling frequency */
    float level;                    /**< current audio volume level */
    bool selected;                  /**< it indicates if the channel is selected */
    /**
     * @brief   user interface elements
     * @{
     */
    QGroupBox *groupbox;            /**< channel group box */
    QLineEdit *label;               /**< field to change the channel label */
    QCheckBox *selectedcheckbox;    /**< selected checkbox object */
    QSlider *levelslider;           /**< volume level slider */
    QWidget *volumeterwidget;       /**< volumeter framework */
    AudioChart *chart;              /**< audio chart object */
    /** @} */
    Channel(QLayout *parent, int index, std::string prefix);
    ~Channel();
    int getIndex();
    void setIndex(int index);
    void setStream(AudioStream *stream);
    void setLabel(std::string label);
    void setSelected(bool state);
    void setLevel(int level);
private:
    int index;
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
    ChannelsList(QWidget *framework, int number, std::string prefix);
    ~ChannelsList();
    Channel *getChannel(int index);
    void addChannel(Channel *channel);
    void deleteChannel(int index);
    void setChannelsNumber(int number);
private:
    QList<Channel *> channels;      /**< list of channels */
    std::string prefix;             /**< user interface prefix */
    QWidget *framework;             /**< user interface framework of channels list */
    QLayout *list;                  /**< user interface layout of the channels list */
    Volumeter *volumeter;           /**< volumeter object */
};

#endif // CHANNELSLIST_H
