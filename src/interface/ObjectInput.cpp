/**
 * @name	ObjectInput.cpp
 * @author	Andrés González Fornell
 * @brief	Single-object input configuration class.
 */

// Class libraries
#include "objects.h"
#include "ui_mainwindow.h"
// System libraries
#include "math.h"
#include "stdlib.h"

/**
 * @brief	ObjectInput constructor.
 * @param   &parent     objects user interface parent
 * @param   index       object index
 * @param   file        source file directory
 */
ObjectInput::ObjectInput(QWidget *parent, int index) {
    // User interface
    QGroupBox *groupbox = new QGroupBox(parent);
    groupbox->setObjectName(QString("object_%1").arg(index));
    groupbox->setMinimumSize(0,0);
    groupbox->setMaximumSize(99999999,99999999);
    QLayout *layout = new QHBoxLayout(groupbox);
    QPushButton *source_select = new QPushButton();
    source_select->setText(QString("object_%1").arg(index));
    layout->addWidget(source_select);
    QCheckBox *recording = new QCheckBox();
    layout->addWidget(recording);
    // Initialization
    this->fromdevice = true;
    this->audioobject = new AudioObject();
    consolelog("objects",progress,"ObjectInput object is created");
}

/**
 * @brief	ObjectInput desctructor.
 */
ObjectInput::~ObjectInput() {
}

/**
 * @brief	It sets the source file.
 * @param   file        file path
 */
void ObjectInput::setFile(string filepath) {
    if (filepath.substr(filepath.size()-4,4) == ".wav"){
        this->file = new QFile(QString::fromStdString(filepath));
        this->fromdevice = false;
    } else {
        this->fromdevice = true;
    }
}

QByteArray ObjectInput::getData() {
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;
    while (!file->atEnd()) {
        QByteArray line = file->readLine();
        return line;
    }
    return 0;
}

void ObjectInput::sendData(float data) {
    audioobject->push(data);
}
