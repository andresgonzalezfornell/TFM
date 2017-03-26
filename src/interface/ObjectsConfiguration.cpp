// Class libraries
#include "objects.h"
#include "ui_mainwindow.h"

/**
 * @brief	ObjectsConfiguration constructor.
 * @param   framework   user interface framework of objects configuration
 * @param   number      number of objects
 * @param   audiochart  master input audio chart
 */
ObjectsConfiguration::ObjectsConfiguration(QWidget *framework, int number, AudioChart *audiochart) {
    this->setNumber(framework, number);
    this->audiochart = audiochart;
    this->master = new AudioSignal();
    consolelog("Objects",LogType::progress,"ObjectsConfiguration object is created");
}

/**
 * @brief	ObjectsConfiguration destructor.
 */
ObjectsConfiguration::~ObjectsConfiguration() {
    consolelog("Objects",LogType::progress,"ObjectsConfiguration object is deleted");
}

/**
 * @brief	It sets a number of objects up.
 * @param   framework   user interface framework of objects configuration
 * @param   number      number of objects
 */
void ObjectsConfiguration::setNumber(QWidget *framework, int number) {
    this->number = number;
    if (objects.size()<number) {
        for (int index = objects.size(); index < this->number;index++) {
            this->objects.push_back(ObjectInput(framework, index));
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
void ObjectsConfiguration::receiveDevice(float value) {
    double amplitude = 0;
    float sample = 0;
    for (int item = 0; item<this->getNumber();item++) {
        ObjectInput object = objects.at(item);
        if(objects.at(item).fromdevice) {
            sample = value;
        } else {
//            filedata = object.getData().toFloat();
        }
        amplitude += sample;
        object.sendData(sample);
    }
    plotAudio(amplitude);
}

/**
 * @brief	It adds a sample to plot input audio.
 * @param   sample
 */
void ObjectsConfiguration::plotAudio(float sample) {
    const int frequencybands = 100; /**< number of frequency bands to plot */
    const float plotperiod = 0.5;   /**< period of audio plotting [s] */
    if(master->size >= AudioSignal::fs()*plotperiod) {
        QVector<QPointF> *points = new QVector<QPointF>();
        vector<float> spectrum = master->getSpectrum(frequencybands);
        for (int f = 0; f < (int)spectrum.size(); f++) {
            points->append(QPointF(AudioSignal::fs()/2*f/spectrum.size(),spectrum[f]));
        }
        audiochart->setPoints(*points);
        master->clear();
    }
    master->addSample(sample);
}
