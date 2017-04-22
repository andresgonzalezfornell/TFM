#ifndef CHANNELS_H
#define CHANNELS_H

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
#include "QLineEdit"
#include "QPushButton"
#include "QComboBox"
#include "QSpinBox"
#include "QCheckBox"
#include "QIcon"
#include "QPixmap"
#include "QVector"
// Classes and local files
#include "../process/File.h"
#include "AudioInfo.h"
#include "AudioChart.h"
#include "../process/AudioSignal.h"
#include "../process/AudioStream.h"
#include "../tools/Logger.h"

/**
 * @class	ConfigurationChannel
 * @author	Andrés González Fornell
 * @brief	Single-object channel configuration class.
 */
class ConfigurationChannel {
public:
    float level;                                /**< current audio volume level */
    WAVFile *file;                            /**< source audio file object */
    bool active;                                /**< it indicates if the channel is active */
    /**
     * @brief   user interface elements
     * @{
     */
    QGroupBox *groupbox;                        /**< channel group box */
    QLineEdit *label;                           /**< field to change the channel label */
    QSlider *levelslider;                       /**< volume level slider */
    QCheckBox *activecheckbox;                  /**< checkbox to indicate if the channel is active */
    QPushButton *loadfile;                      /**< button to load file as source */
    QPushButton *playpause;                     /**< file playback play/pause button */
    QPushButton *info;                          /**< audio file info button */
    QLineEdit *currentsource;                   /**< current source indicator */
    /** @} */
    ConfigurationChannel(QLayout *parent, int index);
    ~ConfigurationChannel();
    int getIndex();
    void setIndex(int index);
    void setLabel(std::string label);
    void setLevel(int level);
    void setActive(bool state);
    void setFile(std::string filepath);
    void openInfo();
    float readData();
    void sendData(float data);
    float getLastValue();
private:
    int index;                                  /**< channel index */
    AudioStream *audiostream;                   /**< audio channel stream */
    std::string name;                           /**< input channel name */
};

/**
 * @class	ChannelsConfiguration
 * @author	Andrés González Fornell
 * @brief	Channels configuration class.
 */
class ChannelsConfiguration : public QObject {
    Q_OBJECT
public:
    static int fs;                              /**< signal sampling frequency */
    ChannelsConfiguration(QWidget *framework, int number);
    ~ChannelsConfiguration();
    ConfigurationChannel *getChannel(int index);
    int getNumber();
    void deleteChannel(int index);
    int getChannelIndex(QObject *element);
private:
    QList<ConfigurationChannel *> channels;     /**< list of channels */
    QWidget *framework;                         /**< user interface framework of channels configuration */
    QLayout *list;                              /**< user interface layout of the channels list */
private slots:
    // Channels configuration controls
    void setNumber(int number);
    // Inputs channels controls
    void setLabel(QString label);
    void setLevel(int level);
    void setActive(bool state);
    void loadFile();
    void openInfo();
};

#endif // CHANNELS_H
