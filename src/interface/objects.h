#ifndef OBJECTS_H
#define OBJECTS_H

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
#include "device.h"
#include "AudioFile.h"
#include "AudioInfo.h"
#include "AudioObject.h"
#include "AudioSignal.h"
#include "AudioChart.h"
#include "../tools/Logger.h"

/**
 * @class	ObjectInput
 * @author	Andrés González Fornell
 * @brief	Single-object input configuration class.
 */
class ObjectInput {
public:
    float level;                    /**< current audio volume level */
    AudioFile *file;                /**< source audio file object */
    bool fromdevice;                /**< it indicates if the audio source is a file (instead of an input device) */
    bool active;                    /**< it indicates if the audio source is active */
    bool paused;                    /**< it indicates if the audio is paused */
    bool preview;                   /**< it indicates if the audio file preview is unmuted */
    /**
     * @brief   user interface elements
     * @{
     */
    QSlider *levelslider;           /**< volume level slider */
    QGroupBox *groupbox;            /**< object group box */
    QLineEdit *label;               /**< field to change the object label */
    QWidget *volumeterwidget;       /**< object level indicator */
    QCheckBox *fromdevicecheckbox;  /**< checkbox to indicate if the source is the input device */
    QCheckBox *activecheckbox;      /**< checkbox to indicate if the object is active */
    QPushButton *loadfile;          /**< button to load file as source */
    QLineEdit *currentsource;       /**< current source indicator */
    QProgressBar *audioprogress;    /**< file playback progress bar */
    QPushButton *playpause;         /**< file playback play/pause button */
    QPushButton *previewbutton;     /**< audio file preview button */
    QPushButton *info;              /**< audio file info button */
    QTimeEdit *audiotime;           /**< file playback current time field */
    /** @} */
    ObjectInput(QLayout *parent, int index);
    ~ObjectInput();
    int getIndex() const;
    void setIndex(int index);
    void setLevel(int level);
    void setLabel(std::string label);
    void setFromDevice(bool state);
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
    int index;                      /**< object index */
    AudioObject *audioobject;       /**< audio object */
    Volumeter *volumeter;           /**< volumeter object */
    std::string name;               /**< input object name */
    float lastvalue;                /**< last received value */
};

/**
 * @class	ObjectsConfiguration
 * @author	Andrés González Fornell
 * @brief	Objects configuration class.
 */
class ObjectsConfiguration : public QObject {
    Q_OBJECT
public:
    ObjectsConfiguration(QWidget *parent, int number);
    ~ObjectsConfiguration();
    ObjectInput *getObject(int index);
    int getNumber();
    void deleteObject(int index);
    int getObjectIndex(QObject *element);
private:
    QWidget *framework;             /**< user interface framework of objects configuration */
    QList<ObjectInput *> objects;   /**< list of object input objects */
    QLayout *layout;                /**< user interface layout of the objects list */
    int number;                     /**< number of objects */
    Volumeter *volumeter;           /**< volumeter object */
private slots:
    // Objects configuration controls
    void setNumber(int number);
    // Inputs objects controls
    void setLevel(int level);
    void setLabel(QString label);
    void setFromDevice(bool state);
    void setActive(bool state);
    void loadFile();
    void switchPlayPause();
    void switchPreview();
    void openInfo();
};

/**
 * @class	Objects
 * @author	Andrés González Fornell
 * @brief	User interface class of Objects framework.
 */
class Objects : public QMainWindow
{
    Q_OBJECT
public:
    static int fs;                                /**< signal sampling frequency [Hz] */
    Device *device;                                 /**< device object */
    ObjectsConfiguration *objectsconfiguration;     /**< objects configuration object */
    AudioChart *audiochart;                         /**< audio chart object */
    Objects(QWidget *framework);
    ~Objects();
    void setfs(int fs);
public slots:
    void receiveDevice(float value);
private:
    AudioSignal *master;                            /**< input master audio signal */
    void addToMaster(float sample);
};

#endif // OBJECTS_H
