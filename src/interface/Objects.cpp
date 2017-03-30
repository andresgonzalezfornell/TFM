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
    float fs = 4000;       // Sampling frequency [Hz]
    framework->setUpdatesEnabled(true);
    this->device = new Device(framework->findChild<QWidget*>("device"),fs);
    QObject::connect(device,SIGNAL(newData(float)),this,SLOT(receiveDevice(float)));
    this->objectsconfiguration = new ObjectsConfiguration(framework->findChild<QWidget *>("objects_list"),framework->findChild<QSpinBox *>("objects_number")->value());
    this->master = new AudioSignal(fs);
    float chartrange[2][2] = {{0,master->fs/2},{0,1e-3}};
    this->audiochart = new AudioChart(framework->findChild<QWidget *>("objects_chart"),chartrange,"",AudioChart::ChartOptions::labelX);
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
    double amplitude = 0;
    float sample = 0;
    for (int item = 0; item<this->objectsconfiguration->getNumber();item++) {
        ObjectInput object = objectsconfiguration->getObject(item);
        if(object.fromdevice) {
            sample = value;
        } else {
//            filedata = object.getData().toFloat();
        }
        amplitude += sample;
        object.sendData(sample);
    }
    addToMaster(amplitude);
}

/**
 * @brief	It adds a sample to plot input audio.
 * @param   sample
 */
void Objects::addToMaster(float sample) {
    const int frequencybands = 100; /**< number of frequency bands to plot */
    const float plotperiod = 0.5;   /**< period of audio plotting [s] */
    if(master->size >= master->fs*plotperiod) {
        QVector<QPointF> *points = new QVector<QPointF>();
        vector<float> frequencies = master->getFrequencies(frequencybands);
        vector<float> spectrum = master->getSpectrum(frequencybands);
        for (int f = 0; f < (int)spectrum.size(); f++) {
            points->append(QPointF(frequencies[f],spectrum[f]));
        }
        audiochart->setPoints(*points);
        master->clear();
    }
    master->addSample(sample);
}
