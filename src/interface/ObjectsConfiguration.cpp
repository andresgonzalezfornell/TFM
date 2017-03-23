/**
 * @name	ObjectsConfiguration.cpp
 * @author	Andrés González Fornell
 * @brief	Objects configuration class.
 */

// Class libraries
#include "objects.h"
#include "ui_mainwindow.h"

/**
 * @brief	ObjectsConfiguration constructor.
 * @param   &parent     objects user interface parent
 * @param   number      number of objects
 */
ObjectsConfiguration::ObjectsConfiguration(QWidget *parent, int number, AudioChart *audiochart) {
    this->setNumber(parent, number);
    this->audiochart = audiochart;
    this->master = new AudioSignal();
    consolelog("Objects",progress,"ObjectsConfiguration object is created");
}

/**
 * @brief	ObjectsConfiguration destructor.
 */
ObjectsConfiguration::~ObjectsConfiguration() {
    consolelog("Objects",progress,"ObjectsConfiguration object is deleted");
}

/**
 * @brief	It sets a number of objects up.
 * @param   &parent     objects user interface parent
 * @param   number      number of objects
 */
void ObjectsConfiguration::setNumber(QWidget *parent, int number) {
    this->number = number;
    if (objects.size()<number) {
        for (int index = objects.size(); index < this->number;index++) {
            this->objects.push_back(ObjectInput(parent, index));
        }
    } else if (objects.size()>number) {
        for (int index = objects.size(); index > this->number;index--) {
            this->objects.pop_back();
        }
    }
}

/**
 * @brief	It gets the number of objects.
 * @return  number of objects
 */
int ObjectsConfiguration::getNumber() {
    return this->number;
}

/**
 * @brief	It is called when device data has been received.
 * @param   devicedata
 */
void ObjectsConfiguration::receiveDevice(quint32 devicedata) {
    const float amplitude_max = 500000;
    double amplitude = 0;
    float data = 0;
    for (int item = 0; item<this->getNumber();item++) {
        ObjectInput object = objects.at(item);
        if(objects.at(item).fromdevice) {
            data = (float)devicedata/amplitude_max;
        } else {
//            data = object.getData().toFloat();
        }
        amplitude += data;
        object.sendData(data);
    }
    plotAudio(amplitude);
}

/**
 * @brief	It adds a sample to plot input audio.
 * @param   sample
 */
void ObjectsConfiguration::plotAudio(float sample) {
    if(master->size >= AudioSignal::fs()) {
        QVector<QPointF> *points = new QVector<QPointF>();
        vector<float> spectrum = master->getSpectrum();
        for (int f = 0; f < master->size; f++) {
            points->append(QPointF(f,spectrum[f]));
        }
        audiochart->setPoints(points);
        master->clear();
    }
    master->addSample(sample);
}
