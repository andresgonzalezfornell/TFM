// Class libraries
#include "objects.h"
#include "ui_mainwindow.h"
// System libraries
#include "math.h"
#include "stdlib.h"

/**
 * @brief	ObjectInput constructor.
 * @param   parent      objects user interface parent
 * @param   index       object index
 */
ObjectInput::ObjectInput(QLayout *parent, int index) {
    // Elements creation
    this->groupbox = new QGroupBox();
    QGridLayout *layout = new QGridLayout(groupbox);
    this->label = new QLineEdit();
    this->volumeterwidget = new QWidget();
    this->fromdevicecheckbox = new QCheckBox();
    this->activecheckbox = new QCheckBox();
    this->currentsource = new QLabel();
    this->loadfile = new QPushButton();
    // Object members
    this->name = QString("Object %1").arg(index).toStdString();
    this->setIndex(index);
    // Elements attributes
    int layout_height = 120;          // height of object configuration interface
    this->groupbox->setMinimumSize(200,0);
    this->groupbox->setMaximumSize(QWIDGETSIZE_MAX,layout_height);
    layout->setSpacing(8);
    this->volumeterwidget->setMaximumSize(80,5);
    this->fromdevicecheckbox->setText("From device");
    this->activecheckbox->setText("Active");
    this->currentsource->setMaximumHeight(15);
    this->loadfile->setText("Load file");
    // Layout
    layout->addWidget(this->label,0,0);
    layout->addWidget(this->volumeterwidget,0,1);
    layout->addWidget(this->fromdevicecheckbox,1,0);
    layout->addWidget(this->activecheckbox,1,1);
    layout->addWidget(this->currentsource,2,0);
    layout->addWidget(this->loadfile,2,1);
    layout->setMargin(10);
    parent->addWidget(this->groupbox);
    // Initialization
    this->setLabel(this->name);
    setFromDevice(true);
    setActive(false);
    this->volumeter = new Volumeter(this->volumeterwidget);
    this->audioobject = new AudioObject();
    consolelog("Objects",LogType::progress,"ObjectInput object is created");
}

/**
 * @brief	ObjectInput desctructor.
 */
ObjectInput::~ObjectInput() {
}

/**
 * @brief   It gets the object index.
 * @return  index
 */
int ObjectInput::getIndex() const
{
    return index;
}

/**
 * @brief   It sets the object index.
 * @param   index
 */
void ObjectInput::setIndex(int index)
{
    this->index = index;
    this->groupbox->setObjectName(QString("objects_%1").arg(index));
    QString prefix = groupbox->objectName() + "_";
    this->label->setObjectName(prefix + "objectname");
    this->volumeterwidget->setObjectName(prefix + "volumeter");
    this->fromdevicecheckbox->setObjectName(prefix + "fromdevice");
    this->activecheckbox->setObjectName(prefix + "active");
    this->currentsource->setObjectName(prefix + "currentsource");
    this->loadfile->setObjectName(prefix + "loadfile");
}

/**
 * @brief	It sets the source file.
 * @param   filepath    file path
 */
void ObjectInput::setFile(string filepath) {
    if (filepath.substr(filepath.size()-4,4) == ".wav"){
        this->file = new QFile(QString::fromStdString(filepath));
        this->fromdevice = false;
    } else {
        this->fromdevice = true;
    }
}

/**
 * @brief   It sets a label to the object name, i.e., group box title and label text.
 * @param   label
 */
void ObjectInput::setLabel(string label) {
    this->groupbox->setTitle("#" + QString::number(this->index) + " " + QString::fromStdString(label));
    this->label->setText(QString::fromStdString(label));
}

/**
 * @brief   It sets if the source is from device.
 * @param   state
 */
void ObjectInput::setFromDevice(bool state) {
    this->fromdevice = state;
    this->fromdevicecheckbox->setChecked(state);
    this->loadfile->setEnabled(!state);
    if(state) {
        this->currentsource->setText("input device");
    } else {
        //this->currentsource->setText(this->file->fileName());
        this->currentsource->setText("file");
    }
    QFont font = QFont();
    font.setPointSize(10);
    font.setItalic(state);
    this->currentsource->setFont(font);
}

/**
 * @brief   It sets if the object is active.
 * @param   state
 */
void ObjectInput::setActive(bool state) {
    this->active = state;
    this->activecheckbox->setChecked(state);
}

/**
 * @brief   It gets data from the object file
 * @return  data
 */
QByteArray ObjectInput::getData() {
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;
    while (!file->atEnd()) {
        QByteArray line = file->readLine();
        return line;
    }
    return 0;
}

/**
 * @brief   It sends data to the audio object associated to this input object
 * @param   data
 */
void ObjectInput::sendData(float data) {
    this->audioobject->push(data);
    this->volumeter->setLevel(data);
}
