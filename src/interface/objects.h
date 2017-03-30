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
#include "QLayout"
#include "QSpinBox"
#include "QCheckBox"
#include "QVector"
// Classes and local files
#include "device.h"
#include "audiochart.h"
#include "../tools/Logger.h"
#include "../module/AudioObject.h"
#include "../module/AudioSignal.h"

/**
 * @class	ObjectInput
 * @author	Andrés González Fornell
 * @brief	Single-object input configuration class.
 */
class ObjectInput {
public:
    bool fromdevice;                /**< it indicates if the audio source is a file (instead of an input device) */
    bool active;                   /**< it indicates if the audio source is active */
    /**
     * @brief   user interface elements
     * @{
     */
    QGroupBox *groupbox;            /**< object group box */
    QLineEdit *label;               /**< field to change the object label */
    QWidget *volumeterwidget;       /**< object level indicator */
    QCheckBox *fromdevicecheckbox;  /**< checkbox to indicate if the source is the input device */
    QCheckBox *activecheckbox;      /**< checkbox to indicate if the object is active */
    QPushButton *loadfile;          /**< button to load file as source */
    QLabel *currentsource;          /**< current source indicator */
    /** @} */
    ObjectInput(QLayout *parent, int index);
    ~ObjectInput();
    int getIndex() const;
    void setIndex(int index);
    void setFile(string filepath);
    void setLabel(string label);
    void setFromDevice(bool state);
    void setActive(bool state);
    QByteArray getData();
    void sendData(float data);
private:
    int index;                      /**< object index */
    QFile *file;                    /**< source file object */
    AudioObject *audioobject;       /**< audio object */
    Volumeter *volumeter;           /**< volumeter object */
    string name;                    /**< input object name */
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
    ObjectInput getObject(int index);
    int getNumber();
    void deleteObject(int index);
    ObjectInput getParentObject(QObject *element);
private:
    QWidget *framework;             /**< user interface framework of objects configuration */
    QList<ObjectInput> objects;     /**< list of object input objects */
    QLayout *layout;                /**< user interface layout of the objects list */
    int number;                     /**< number of objects */
    Volumeter *volumeter;           /**< volumeter object */
private slots:
    // Objects configuration controls
    void setNumber(int number);
    // Inputs objects controls
    void setLabel(QString label);
    void setFromDevice(bool state);
    void setActive(bool state);
    void loadFile();
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
    Device *device;                                 /**< device object */
    ObjectsConfiguration *objectsconfiguration;     /**< objects configuration object */
    AudioChart *audiochart;                         /**< audio chart object */
    Objects(QWidget *framework);
    ~Objects();
public slots:
    void receiveDevice(float value);
private:
    AudioSignal *master;                            /**< input master audio signal */
    void addToMaster(float sample);
};

#endif // OBJECTS_H
