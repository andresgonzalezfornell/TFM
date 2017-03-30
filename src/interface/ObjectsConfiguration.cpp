// Class libraries
#include "objects.h"
#include "ui_mainwindow.h"

/**
 * @brief	ObjectsConfiguration constructor.
 * @param   framework   user interface framework of objects configuration
 * @param   number      number of objects
 */
ObjectsConfiguration::ObjectsConfiguration(QWidget *framework, int number) {
    this->framework = framework;
    this->layout = framework->findChild<QLayout *>("objects_list_layout");
    this->setNumber(number);
    consolelog("Objects",LogType::progress,"ObjectsConfiguration object is created");
}

/**
 * @brief	ObjectsConfiguration destructor.
 */
ObjectsConfiguration::~ObjectsConfiguration() {
    consolelog("Objects",LogType::progress,"ObjectsConfiguration object is deleted");
}

/**
 * @brief   It gets a object input.
 * @param   index       object index
 * @return  object
 */
ObjectInput ObjectsConfiguration::getObject(int index) {
    return this->objects.at(index);
}

/**
 * @brief	It sets a number of objects up.
 * @param   number      number of objects
 */
void ObjectsConfiguration::setNumber(int number) {
    this->number = number;
    if (objects.size()<number) {
        for (int index = objects.size(); index < this->number;index++) {
            ObjectInput object = ObjectInput(this->layout, index);
            this->objects.push_back(object);
            // User interface control
            QObject::connect(object.label,SIGNAL(textChanged(QString)),this,SLOT(setLabel(QString)));
            QObject::connect(object.fromdevicecheckbox,SIGNAL(clicked(bool)),this,SLOT(setFromDevice(bool)));
            QObject::connect(object.activecheckbox,SIGNAL(clicked(bool)),this,SLOT(setActive(bool)));
            QObject::connect(object.loadfile,SIGNAL(clicked()),this,SLOT(loadFile()));
        }
    } else if (objects.size()>number) {
        for (int index = objects.size()-1; index >= this->number;index--) {
            this->deleteObject(index);
            consolelog("ObjectsConfiguration",LogType::progress,"object " + std::to_string(index) + " is deleted");
        }
    }
    consolelog("ObjectsConfiguration",LogType::interaction,"number of objects has been changed to " + std::to_string(number));
}

/**
 * @brief	It gets the number of objects.
 * @return  number of objects
 */
int ObjectsConfiguration::getNumber() {
    return this->number;
}

/**
 * @brief   It deletes a specific object.
 * @param   index       object index
 */
void ObjectsConfiguration::deleteObject(int index) {
    delete this->framework->findChild<QGroupBox *>(QString("objects_%1").arg(index));
    this->objects.removeAt(index);
    consolelog("ObjectsInput",LogType::progress,"ObjectInput object is deleted");
}

/**
 * @brief   It gets the parent input object of a user interface element object.
 * @param   object      user interface element object
 * @return  index
 */
ObjectInput ObjectsConfiguration::getParentObject(QObject *element) {
    string prefix = "objects_";
    int index = std::stoi(element->objectName().toStdString().substr(element->objectName().toStdString().find(prefix) + prefix.length(),1));
    return getObject(index);
}

/**
 * @name    Inputs objects interface slots
 * @brief   User interface control functions of inputs objects.
 * @{
 */

/**
 * @brief   Input object slots for setting the object label.
 * @param   label
 */
void ObjectsConfiguration::setLabel(QString label) {
    this->getParentObject(QObject::sender()).setLabel(label.toStdString());
    consolelog("ObjectsConfiguration",LogType::interaction,"label of object " + std::to_string(this->getParentObject(QObject::sender()).getIndex()) + " has been changed to \"" + label.toStdString() + "\"");
}

/**
 * @brief   Input object slots when fromdevice checkbox has been changed.
 * @param   state       current checkbox state
 */
void ObjectsConfiguration::setFromDevice(bool state) {
    ObjectInput object = this->getParentObject(QObject::sender());
    object.setFromDevice(state);
    string message = "object " + std::to_string(object.getIndex()) + " source has been changed to be obtained ";
    if(state) {
        message += "from the device";
    } else {
        message += "from a file";
    }
    consolelog("ObjectsConfiguration",LogType::interaction,message);
}

/**
 * @brief   Input object slots when active checkbox has been changed.
 * @param   state       current checkbox state
 */
void ObjectsConfiguration::setActive(bool state) {
    ObjectInput object = this->getParentObject(QObject::sender());
    object.setActive(state);
    string message = "object " + std::to_string(object.getIndex()) + " has been changed to ";
    if(state) {
        message += "active";
    } else {
        message += "inactive";
    }
    consolelog("ObjectsConfiguration",LogType::interaction,message);
}

/**
 * @brief   Input object slots for loading a file.
 */
void ObjectsConfiguration::loadFile() {
}

/** @} */
