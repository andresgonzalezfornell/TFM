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
ObjectInput *ObjectsConfiguration::getObject(int index) {
    return this->objects[index];
}

/**
 * @brief	It sets a number of objects up.
 * @param   number      number of objects
 */
void ObjectsConfiguration::setNumber(int number) {
    this->number = number;
    if (objects.size()<number) {
        for (int index = objects.size(); index < this->number;index++) {
            ObjectInput *object = new ObjectInput(this->layout, index);
            this->objects.push_back(object);
            // User interface control
            QObject::connect(object->levelslider,SIGNAL(valueChanged(int)),this,SLOT(setLevel(int)));
            QObject::connect(object->label,SIGNAL(textChanged(QString)),this,SLOT(setLabel(QString)));
            QObject::connect(object->fromdevicecheckbox,SIGNAL(clicked(bool)),this,SLOT(setFromDevice(bool)));
            QObject::connect(object->activecheckbox,SIGNAL(clicked(bool)),this,SLOT(setActive(bool)));
            QObject::connect(object->loadfile,SIGNAL(released()),this,SLOT(loadFile()));
            QObject::connect(object->playpause,SIGNAL(released()),this,SLOT(switchPlayPause()));
            QObject::connect(object->previewbutton,SIGNAL(released()),this,SLOT(switchPreview()));
            QObject::connect(object->info,SIGNAL(released()),this,SLOT(openInfo()));
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
 * @brief   It gets the input object index of a user interface element object.
 * @param   element     user interface element object
 * @return  index
 */
int ObjectsConfiguration::getObjectIndex(QObject *element) {
    std::string prefix = "objects_";
    return std::stoi(element->objectName().toStdString().substr(element->objectName().toStdString().find(prefix) + prefix.length(),1));
}

/**
 * @name    Inputs objects interface slots
 * @brief   User interface control functions of inputs objects.
 * @{
 */

/**
 * @brief   Input object slots for setting the object level.
 * @param   level
 */
void ObjectsConfiguration::setLevel(int level) {
    QObject::sender()->blockSignals(true);
    int index = this->getObjectIndex(QObject::sender());
    this->objects[index]->setLevel(level);
    consolelog("ObjectsConfiguration",LogType::interaction,"volume level of object " + std::to_string(index) + " set to " + std::to_string(level) + "%");
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Input object slots for setting the object label.
 * @param   label
 */
void ObjectsConfiguration::setLabel(QString label) {
    QObject::sender()->blockSignals(true);
    int index = this->getObjectIndex(QObject::sender());
    this->objects[index]->setLabel(label.toStdString());
    consolelog("ObjectsConfiguration",LogType::interaction,"label of object " + std::to_string(index) + " has been changed to \"" + label.toStdString() + "\"");
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Input object slots when fromdevice checkbox has been changed.
 * @param   state       current checkbox state
 */
void ObjectsConfiguration::setFromDevice(bool state) {
    QObject::sender()->blockSignals(true);
    int index = this->getObjectIndex(QObject::sender());
    objects[index]->setFromDevice(state);
    std::string message = "object " + std::to_string(index) + " source has been changed to be obtained ";
    if(state) {
        message += "from the device";
    } else {
        message += "from a file";
    }
    consolelog("ObjectsConfiguration",LogType::interaction,message);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Input object slots when active checkbox has been changed.
 * @param   state       current checkbox state
 */
void ObjectsConfiguration::setActive(bool state) {
    QObject::sender()->blockSignals(true);
    int index = this->getObjectIndex(QObject::sender());
    objects[index]->setActive(state);
    std::string message = "object " + std::to_string(index) + " has been changed to ";
    if(state) {
        message += "active";
    } else {
        message += "inactive";
    }
    consolelog("ObjectsConfiguration",LogType::interaction,message);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Input object slots for loading a file.
 */
void ObjectsConfiguration::loadFile() {
    QObject::sender()->blockSignals(true);
    int index = this->getObjectIndex(QObject::sender());
    consolelog("ObjectsConfiguration",LogType::interaction,"selecting source file for object " + std::to_string(index));
    std::string filepath = QFileDialog::getOpenFileName(this->framework, "Load audio file","","*.wav").toStdString();
    if(filepath=="") {
        consolelog("ObjectsConfiguration",LogType::interaction,"canceling source file selection for object " + std::to_string(index));
    } else {
        consolelog("ObjectsConfiguration",LogType::interaction,"selected \"" + filepath + "\" as source file for object " + std::to_string(index));
        this->objects[index]->setFile(filepath);
    }
    QObject::sender()->blockSignals(false);
}

/**
 * @brief	Input object slots for play/pause action.
 */
void ObjectsConfiguration::switchPlayPause() {
    QObject::sender()->blockSignals(true);
    int index = this->getObjectIndex(QObject::sender());
    if (this->objects[index]->paused) {
        this->objects[index]->playFile();
        consolelog("ObjectsConfiguration",LogType::interaction,"object " + std::to_string(index) + " has been resumed");
    } else {
        this->objects[index]->pauseFile();
        consolelog("ObjectsConfiguration",LogType::interaction,"object " + std::to_string(index) + " has been paused");
    }
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Input object slots for preview action.
 */
void ObjectsConfiguration::switchPreview() {
    QObject::sender()->blockSignals(true);
    int index = this->getObjectIndex(QObject::sender());
    if (this->objects[index]->preview) {
        this->objects[index]->mutePreview();
        consolelog("ObjectsConfiguration",LogType::interaction,"object " + std::to_string(index) + " preview has been muted");
    } else {
        this->objects[index]->unmutePreview();
        consolelog("ObjectsConfiguration",LogType::interaction,"object " + std::to_string(index) + " preview has been unmuted");
    }
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Input object slots for info action.
 */
void ObjectsConfiguration::openInfo() {
    QObject::sender()->blockSignals(true);
    int index = this->getObjectIndex(QObject::sender());
    AudioInfo info;
    info.setFile(this->objects[index]->file);
    info.setWindowTitle(this->objects[index]->label->text() + QString(" - Info"));
    info.exec();
    consolelog("ObjectsConfiguration",LogType::interaction,"object " + std::to_string(index) + " is now showing info");
    QObject::sender()->blockSignals(false);
}

/** @} */
