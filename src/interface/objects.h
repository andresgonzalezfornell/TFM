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
class ObjectInput  {
public:
    bool fromdevice;            /**< it indicates if the audio source is a file (instead of an input device) */
    bool actived;               /**< it indicates if the audio source is actived */
    ObjectInput(QLayout *parent, int index);
    ~ObjectInput();
    void setFile(string filepath);
    QByteArray getData();
    void sendData(const char *datapointer, qint64 datalength);
private:
    QFile *file;                /**< source file object */
    AudioObject *audioobject;   /**< audio object */
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
    void setNumber(int number);
    int getNumber();
    void sendOutput();
private:
    QList<ObjectInput> objects;     /**< list of object input objects */
    QWidget *framework;             /**< user interface framework of objects configuration */
    QLayout *layout;                /**< user interface layout of the objects list */
    int number;                     /**< number of objects */
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
