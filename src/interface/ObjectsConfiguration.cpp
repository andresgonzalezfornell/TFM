/**
 * @name	ObjectsConfiguration.cpp
 * @author	Andrés González Fornell
 * @brief	Objects configuration class.
 */

// Class libraries
#include "objects.h"
#include "ui_mainwindow.h"
// System libraries
#include "math.h"
#include "stdlib.h"

/**
 * @brief	ObjectsConfiguration constructor.
 * @param   &parent     objects user interface parent
 * @param   number      number of objects
 */
ObjectsConfiguration::ObjectsConfiguration(QWidget *parent, int number) {
    this->setNumber(parent, number);
    consolelog("objects",progress,"ObjectsConfiguration object is created");
}

/**
 * @brief	ObjectsConfiguration destructor.
 */
ObjectsConfiguration::~ObjectsConfiguration() {
    consolelog("objects",progress,"ObjectsConfiguration object is deleted");
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
 * @param   data
 */
void ObjectsConfiguration::receiveDevice(quint32 data) {
    for (int item = 0; item<this->getNumber();item++) {
        if(objects.at(item).fromdevice) {
            ObjectInput object = objects.at(item);
            object.sendData(data);
        }
    }
    consolelog("ObjectsConfiguration",info,std::to_string(data));
}

/**
 * @brief	It sends the output audio from the current objects configuration to the audio input
 * @param   data
 */
void ObjectsConfiguration::sendOutput() {
    double audiosignal = 0;
    double data = 0;
    for (int item = 0; item<this->getNumber();item++) {
        ObjectInput object = objects.at(item);
//        data = object.getData();
//        object.sendData(data);
//        audiosignal += data;
    }
    consolelog("ObjectsConfiguration",info,std::to_string(audiosignal));
}
