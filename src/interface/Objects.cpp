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
    device = new Device(framework->findChild<QWidget*>("device"));
    float chartrange[2][2] = {{0,AudioSignal::fs()/2},{0,500000}};
    audiochart = new AudioChart(framework->findChild<QWidget *>("objects_chart"),chartrange,"Audio objects",AudioChart::ChartOptions::labelX);
    objectsconfiguration = new ObjectsConfiguration(framework->findChild<QWidget *>("objects_configuration"),framework->findChild<QSpinBox *>("objects_number")->value(),audiochart);
    QObject::connect(device->channel, SIGNAL(newData(float)), objectsconfiguration, SLOT(receiveDevice(float)));
    consolelog("Objects",LogType::progress,"Objects object is created");
}

/**
 * @brief	Objects destructor.
 */
Objects::~Objects() {
    consolelog("Objects",LogType::progress,"Objects object is deleted");
}
