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
    // User interface
    int layout_height = 150;          // height of object configuration interface
    QGroupBox *groupbox = new QGroupBox();
    groupbox->setObjectName(QString("objects_%1").arg(index));
    groupbox->setTitle(QString("Object %1").arg(index));
    groupbox->setMinimumSize(200,layout_height);
    groupbox->setMaximumSize(QWIDGETSIZE_MAX,layout_height);
    // Elements creation
    QGridLayout *layout = new QGridLayout(groupbox);
    QLineEdit *objectname = new QLineEdit();
    QWidget *volumeter = new QWidget();
    QCheckBox *fromdevicecheckbox = new QCheckBox();
    QCheckBox *activedcheckbox = new QCheckBox();
    QPushButton *loadfile = new QPushButton();
    QPushButton *deleteobject = new QPushButton();
    QLabel *currentsource = new QLabel();
    // Object name of elements
    QString prefix = QString("objects_%1_").arg(index);
    objectname->setObjectName(prefix + "objectname");
    volumeter->setObjectName(prefix + "volumeter");
    fromdevicecheckbox->setObjectName(prefix + "fromdevice");
    activedcheckbox->setObjectName(prefix + "actived");
    loadfile->setObjectName(prefix + "loadfile");
    deleteobject->setObjectName(prefix + "deleteobject");
    currentsource->setObjectName(prefix + "currentsource");
    // Elements attributes
    objectname->setText(QString("Object %1").arg(index));
    fromdevicecheckbox->setText("From device");
    fromdevicecheckbox->setChecked(true);
    activedcheckbox->setText("Actived");
    activedcheckbox->setChecked(false);
    loadfile->setText("Load file");
    deleteobject->setText("Delete");
    currentsource->setText("input device");
    currentsource->setMaximumHeight(30);
    // Layout
    layout->addWidget(objectname,0,0);
    layout->addWidget(volumeter,0,1);
    layout->addWidget(fromdevicecheckbox,1,0);
    layout->addWidget(activedcheckbox,1,1);
    layout->addWidget(loadfile,2,0);
    layout->addWidget(deleteobject,2,1);
    layout->addWidget(currentsource,3,0);
    layout->setMargin(10);
    parent->addWidget(groupbox);
    // Initialization
    this->fromdevice = fromdevicecheckbox->isChecked();
    this->actived = activedcheckbox->isChecked();
    this->audioobject = new AudioObject();
    consolelog("Objects",LogType::progress,"ObjectInput object is created");
}

/**
 * @brief	ObjectInput desctructor.
 */
ObjectInput::~ObjectInput() {
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
void ObjectInput::sendData(const char *datapointer, qint64 datalength) {




    audioobject->push(data);
}
