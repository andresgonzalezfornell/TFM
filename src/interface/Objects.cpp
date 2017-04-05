// Class libraries
#include "objects.h"
#include "ui_mainwindow.h"
// System libraries
#include "math.h"
#include "stdlib.h"

/**
 * @brief	Objects constructor.
 * @param   framework   user interface framework of objects
 */
Objects::Objects(QWidget *framework) {
    framework->setUpdatesEnabled(true);
    this->device = new Device(framework->findChild<QWidget*>("device"),Objects::fs);
    this->master = new AudioSignal(Objects::fs);
    float chartrange[2][2] = {{0,(float)Objects::fs/2},{0,1e-3}};
    this->audiochart = new AudioChart(framework->findChild<QWidget *>("objects_chart"),chartrange,"",AudioChart::ChartOptions::labelX);
    QObject::connect(device,SIGNAL(newData(float)),this,SLOT(receiveDevice(float)));
    this->objectsconfiguration = new ObjectsConfiguration(framework->findChild<QWidget *>("objects_list"),framework->findChild<QSpinBox *>("objects_number")->value());
    // User interface slots
    QObject::connect(framework->findChild<QSpinBox *>("objects_number"),SIGNAL(valueChanged(int)),this->objectsconfiguration,SLOT(setNumber(int)));
    consolelog("Objects",LogType::progress,"Objects object is created");
}

/**
 * @brief	Objects destructor.
 */
Objects::~Objects() {
    consolelog("Objects",LogType::progress,"Objects object is deleted");
}

/**
 * @brief	It is called when device data has been received.
 * @param   value
 */
void Objects::receiveDevice(float value) {
    float amplitude = 0;
    float sample = 0;
    for (int item = 0; item<this->objectsconfiguration->getNumber();item++) {
        if (this->objectsconfiguration->getObject(item)->active) {                              //  is active
            if(this->objectsconfiguration->getObject(item)->fromdevice) {                       //      from device
                sample = value;
                this->objectsconfiguration->getObject(item)->sendData(sample);
                amplitude += this->objectsconfiguration->getObject(item)->getLastValue();
            } else {                                                                            //      from file
                if(this->objectsconfiguration->getObject(item)->file->exists()) {               //          file exists
                    sample = this->objectsconfiguration->getObject(item)->readData();
                    this->objectsconfiguration->getObject(item)->sendData(sample);
                    amplitude += this->objectsconfiguration->getObject(item)->getLastValue();
                }
            }
        }
    }
    addToMaster(amplitude);
}

/**
 * @brief	It adds a sample to plot input audio.
 * @param   sample
 */
void Objects::addToMaster(float sample) {
    const int frequencybands = 100; /**< number of frequency bands to plot */
    if(this->master->size >= 20000) {
        QVector<QPointF> *points = new QVector<QPointF>();
        std::vector<float> frequencies = this->master->getFrequencies(frequencybands);
        std::vector<float> spectrum = this->master->getSpectrum(frequencybands);
        for (int f = 0; f < (int)spectrum.size(); f++) {
            points->append(QPointF(frequencies[f],spectrum[f]));
        }
        this->audiochart->setPoints(*points);
        this->master->clear();
    }
    this->master->addSample(sample);
}

/**
 * @brief   It sets the signal sampling frequency
 * @param   fs          signal sampling frequency [Hz]
 */
void Objects::setfs(int fs) {
    Objects::fs = fs;
    if(this != NULL) {
        this->device->initialize(fs);
        this->master = new AudioSignal(fs);
        float chartrange[2][2] = {{0,(float)master->fs/2},{0,1e-3}};
        audiochart->setRange(chartrange);
        consolelog("Objects",LogType::progress,"objects signal sampling frequency has been changed to " + std::to_string(fs));
    }
}
