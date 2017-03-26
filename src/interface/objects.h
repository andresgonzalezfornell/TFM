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
    ObjectInput(QWidget *parent, int index);
    ~ObjectInput();
    void setFile(string filepath);
    QByteArray getData();
    void sendData(float data);
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
    ObjectsConfiguration(QWidget *parent, int number, AudioChart *audiochart);
    ~ObjectsConfiguration();
    void setNumber(QWidget *parent, int number);
    int getNumber();
    void sendOutput();
private:
    QList<ObjectInput> objects;                 /**< list of object input objects */
    int number;                                 /**< number of objects */
    AudioChart *audiochart;                     /**< audio chart object */
    AudioSignal *master;                        /**< input master audio signal */
    void plotAudio(float sample);
public slots:
    void receiveDevice(float value);
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
};

#endif // OBJECTS_H
