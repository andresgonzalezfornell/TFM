#ifndef OBJECTS_H
#define OBJECTS_H

// System libraries
#include "string"
// Qt libraries
#include "QMainWindow"
#include "QObject"
#include "QFile"
#include "QLayout"
#include "QSpinBox"
#include "QCheckBox"
// Classes and local files
#include "device.h"
#include "../tools/Logger.h"
#include "../module/AudioObject.h"

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
    ObjectsConfiguration(QWidget *parent, int number);
    ~ObjectsConfiguration();
    void setNumber(QWidget *parent, int number);
    int getNumber();
    void sendOutput();
public slots:
    void receiveDevice(quint32);
private:
    QList<ObjectInput> objects;
    int number;
};

class Objects : public QMainWindow
{
    Q_OBJECT
public:
    Objects(QWidget *ui_device);
    ~Objects();
    Device *device;
    ObjectsConfiguration *objectconfiguration;
private:
    QWidget *ui_objects;
};

#endif // OBJECTS_H
