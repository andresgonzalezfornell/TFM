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

class ObjectInput  {
public:
    ObjectInput(QWidget *parent, int index);
    ~ObjectInput();
    bool fromdevice;
    void setFile(string filepath);
    QByteArray getData();
    void sendData(float data);
private:
    QFile *file;
    AudioObject *audioobject;

};

class ObjectsConfiguration : public QObject {
    Q_OBJECT
public:
    ObjectsConfiguration(QWidget *parent, int number, AudioChart *audiochart);
    ~ObjectsConfiguration();
    void setNumber(QWidget *parent, int number);
    int getNumber();
    void sendOutput();
public slots:
    void receiveDevice(quint32 devicedata);
private:
    QList<ObjectInput> objects;
    int number;
    AudioChart *audiochart;
    AudioSignal *master;
    void plotAudio(float sample);
};

class Objects : public QMainWindow
{
    Q_OBJECT
public:
    Objects(QWidget *ui_device);
    ~Objects();
    Device *device;
    ObjectsConfiguration *objectconfiguration;
    AudioChart *audiochart;
private:
    QWidget *ui_objects;
};

#endif // OBJECTS_H
