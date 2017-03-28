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
    return objects.at(index);
}

/**
 * @brief	It sets a number of objects up.
 * @param   number      number of objects
 */
void ObjectsConfiguration::setNumber(int number) {
    this->number = number;
    if (objects.size()<number) {
        for (int index = objects.size(); index < this->number;index++) {
            this->objects.push_back(ObjectInput(this->layout, index));
        }
    } else if (objects.size()>number) {
        for (int index = objects.size()-1; index >= this->number;index--) {
            delete this->framework->findChild<QGroupBox *>(QString("objects_%1").arg(index));
            this->objects.pop_back();
            consolelog("ObjectsConfiguration",LogType::progress,"object " + std::to_string(index) + " is deleted");
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
