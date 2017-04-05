// Class libraries
#include "objects.h"
#include "ui_mainwindow.h"
// System libraries
#include "math.h"
#include "stdlib.h"

/**
 * @brief	ObjectInput constructor.
 * @param   parent          objects user interface parent
 * @param   index           object index
 */
ObjectInput::ObjectInput(QLayout *parent, int index) {
    // Elements creation
    this->groupbox = new QGroupBox();
    QGridLayout *layout = new QGridLayout(groupbox);
    this->label = new QLineEdit();
    this->levelslider = new QSlider(Qt::Horizontal);
    this->activecheckbox = new QCheckBox();
    this->volumeterwidget = new QWidget();
    this->fromdevicecheckbox = new QCheckBox();
    this->currentsource = new QLineEdit();
    this->loadfile = new QPushButton();
    this->audioprogress = new QProgressBar();
    QHBoxLayout *filelayout = new QHBoxLayout();
    this->playpause = new QPushButton();
    this->previewbutton = new QPushButton();
    this->info = new QPushButton();
    this->audiotime = new QTimeEdit();
    // Initialization
    this->file = NULL;
    this->name = QString("Object %1").arg(index).toStdString();
    this->audiostream = new AudioStream(Objects::fs);
    this->setIndex(index);
    this->setLabel(this->name);
    this->setLevel(70);
    this->setActive(false);
    this->volumeter = new Volumeter(this->volumeterwidget,Objects::fs);
    this->setFromDevice(true);
    this->setFile("");
    this->pauseFile();
    this->mutePreview();
    this->info->setIcon(QIcon(QPixmap(":/icons/info")));
    // Elements attributes
    int layout_height = 200;          // height of object configuration interface
    this->groupbox->setMinimumSize(200,0);
    this->groupbox->setMaximumSize(QWIDGETSIZE_MAX,layout_height);
    layout->setSpacing(8);
    this->label->setClearButtonEnabled(true);
    this->levelslider->setMinimum(0);
    this->levelslider->setMaximum(100);
    this->activecheckbox->setText("Active");
    this->volumeterwidget->setMaximumSize(80,5);
    this->fromdevicecheckbox->setText("From device");
    this->currentsource->setMaximumHeight(15);
    this->currentsource->setReadOnly(true);
    this->loadfile->setText("Load file");
    filelayout->setSpacing(10);
    this->playpause->setMaximumSize(20,20);
    this->previewbutton->setMaximumSize(20,20);
    this->info->setMaximumSize(20,20);
    this->audiotime->setDisplayFormat(QString("mm:ss:zzz"));
    // Layout
    layout->addWidget(this->label,0,0);
    layout->addWidget(this->levelslider,0,1);
    layout->addWidget(this->activecheckbox,1,0);
    layout->addWidget(this->volumeterwidget,1,1);
    layout->addWidget(this->fromdevicecheckbox,2,0);
    layout->addWidget(this->currentsource,2,1);
    layout->addWidget(this->loadfile,3,0);
    layout->addWidget(this->audioprogress,3,1);
    filelayout->addWidget(this->playpause);
    filelayout->addWidget(this->previewbutton);
    filelayout->addWidget(this->info);
    layout->addLayout(filelayout,4,0);
    layout->addWidget(this->audiotime,4,1);
    layout->setMargin(10);
    parent->addWidget(this->groupbox);
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
int ObjectInput::getIndex() const {
    return index;
}

/**
 * @brief   It sets the object index.
 * @param   index
 */
void ObjectInput::setIndex(int index) {
    this->index = index;
    this->groupbox->setObjectName(QString("objects_%1").arg(index));
    QString prefix = groupbox->objectName() + "_";
    this->label->setObjectName(prefix + "objectname");
    this->levelslider->setObjectName(prefix + "level");
    this->activecheckbox->setObjectName(prefix + "active");
    this->volumeterwidget->setObjectName(prefix + "volumeter");
    this->fromdevicecheckbox->setObjectName(prefix + "fromdevice");
    this->currentsource->setObjectName(prefix + "currentsource");
    this->loadfile->setObjectName(prefix + "loadfile");
    this->audioprogress->setObjectName(prefix + "fileprogress");
    this->playpause->setObjectName(prefix + "playpause");
    this->previewbutton->setObjectName(prefix + "preview");
    this->info->setObjectName(prefix + "info");
    this->audiotime->setObjectName(prefix + "currenttime");
}

/**
 * @brief   It sets the input object volume level.
 * @param   level           selected volume level
 */
void ObjectInput::setLevel(int level) {
    this->level = (float)level/100;
    this->levelslider->setValue(level);
}

/**
 * @brief   It sets a label to the object name, i.e., group box title and label text.
 * @param   label
 */
void ObjectInput::setLabel(std::string label) {
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
        this->loadfile->hide();
        this->audioprogress->hide();
        this->playpause->hide();
        this->info->hide();
        this->previewbutton->hide();
        this->audiotime->hide();
    } else {
        if(file==NULL) {
            this->currentsource->setText("load a file");
        } else {
            this->currentsource->setText(QString::fromStdString(this->file->getFilepath()));
        }
        this->loadfile->show();
        this->audioprogress->show();
        this->playpause->show();
        this->info->show();
        this->previewbutton->show();
        this->audiotime->show();
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
 * @brief	It sets the source file.
 * @param   filepath        file path
 */
void ObjectInput::setFile(std::string filepath) {
    if(filepath == "") {
        if(this->file->exists()) {
            delete this->file;
        }
        this->playpause->setEnabled(false);
        this->info->setEnabled(false);
        this->previewbutton->setEnabled(false);
        this->audiotime->setEnabled(false);
    } else {
        this->file = new AudioFile(filepath);
        this->currentsource->setText(QString::fromStdString(filepath));
        this->playpause->setEnabled(true);
        this->info->setEnabled(true);
        this->previewbutton->setEnabled(true);
        this->audiotime->setEnabled(true);
    }
}

/**
 * @brief   It sets the current playback progress.
 * @param   value           value of completion (from 0 to 1)
 */
void ObjectInput::setAudioProgress(float value) {

}

/**
 * @brief   It resumes file playback.
 */
void ObjectInput::playFile() {
    this->paused = false;
    this->playpause->setIcon(QIcon(QPixmap(":/icons/pause")));
    this->audioprogress->setEnabled(this->paused);
}

/**
 * @brief   It pauses file playback.
 */
void ObjectInput::pauseFile() {
    this->paused = true;
    this->playpause->setIcon(QIcon(QPixmap(":/icons/play")));
    this->audioprogress->setEnabled(this->paused);
}

/**
 * @brief   It unmutes file playback preview.
 */
void ObjectInput::unmutePreview() {
    this->preview = true;
    this->previewbutton->setIcon(QIcon(QPixmap(":/icons/preview_mute")));
}

/**
 * @brief   It mutes file playback preview.
 */
void ObjectInput::mutePreview() {
    this->preview = false;
    this->previewbutton->setIcon(QIcon(QPixmap(":/icons/preview_unmute")));
}

/**
 * @brief   It sets the current playback time.
 * @param   time            time as QTime object
 */
void ObjectInput::setAudioTime(QTime time) {
    this->audiotime->setTime(time);
}

/**
 * @brief   It sets the current playback time.
 * @param   milliseconds    time in milliseconds [ms]
 */
void ObjectInput::setAudioTime(int milliseconds) {
    int seconds = (int)((float)milliseconds/1000);
    int minutes = (int)((float)seconds/60);
    int hours = (int)((float)minutes/60);
    milliseconds %= 1000;
    QTime time = QTime(hours,minutes,seconds,milliseconds);
    this->audiotime->setTime(time);
}

/**
 * @brief   It reads data from the object file.
 * @return  data
 */
float ObjectInput::readData() {
    if(this->file->exists()) {
        if (this->paused) {
            return 0;
        } else {
            float value = 0;
            quint16 data_16 = 0;
            quint32 data_32 = 0;
            quint32 amplitude = pow(2,this->file->header.bitspersample-1);
            int bytespersample = (int)(this->file->header.bitspersample / 8);
            switch (bytespersample) {
            case 2:
            default:
                data_16 = (qint16)this->file->readDataNumber(bytespersample,AudioFile::Endianess::littleendian);
                value = (float)data_16/(float)amplitude;
                break;
            case 4:
                data_32 = (qint32)this->file->readDataNumber(bytespersample,AudioFile::Endianess::littleendian);
                value = data_32/amplitude;
                break;
            }
            int bytes = this->file->cursor-44;
            float seconds = (float)bytes / (float)this->file->header.byterate;
            int milliseconds = (int)(1000*seconds);
            // Setting playback time every 1000ms
            if((milliseconds%1000 == 0) && (milliseconds != this->audiotime->time().msec())) {
                this->setAudioTime(milliseconds);
            }
            return value;
        }
    } else {
        consolelog("ObjectInput",LogType::error,"error while reading data because audio file does not exist");
        return 0;
    }
}

/**
 * @brief   It sends data to the audio object associated to this input object.
 * @param   data
 */
void ObjectInput::sendData(float data) {
    data *= this->level;
    this->lastvalue = data;
    this->audiostream->push(data);
    this->volumeter->addSample(data);
}

/**
 * @brief   It gets the last data value sent to the audio object.
 * @return  last value
 */
float ObjectInput::getLastValue() {
    return this->lastvalue;
}
