/**
 * @name	Objects.cpp
 * @author	Andrés González Fornell
 * @brief	User interface class of Objects framework.
 */

// Class libraries
#include "objects.h"
#include "ui_mainwindow.h"
// System libraries
#include "math.h"
#include "stdlib.h"

/**
 * @brief	Objects constructor.
 * @param   &framework  user interface framework of objects
 */
Objects::Objects(QWidget *framework) {
    framework->setUpdatesEnabled(true);
    device = new Device(framework->findChild<QWidget*>("device"));
    objectconfiguration = new ObjectsConfiguration(framework->findChild<QWidget *>("objects_configuration"),framework->findChild<QSpinBox *>("objects_number")->value());
    QObject::connect(device->channel, SIGNAL(newData(quint32)), objectconfiguration, SLOT(receiveDevice(quint32)));
    consolelog("objects",progress,"Objects object is created");
}

/**
 * @brief	Objects destructor.
 */
Objects::~Objects() {
    consolelog("objects",progress,"Objects object is deleted");
}
