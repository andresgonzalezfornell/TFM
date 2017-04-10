// Class libraries
#include "ChannelsConfiguration.h"

int ChannelsConfiguration::fs;

/**
 * @brief	ChannelsConfiguration constructor.
 * @param   framework   user interface framework of channels configuration
 * @param   number      number of channels
 */
ChannelsConfiguration::ChannelsConfiguration(QWidget *framework, int number) {
    // User interface slots
    QObject::connect(framework->findChild<QSpinBox *>("channels_number"),SIGNAL(valueChanged(int)),this,SLOT(setNumber(int)));
    this->framework = framework;
    this->list = framework->findChild<QLayout *>("channels_list_layout");
    this->setNumber(number);
    consolelog("ChannelsConfiguration",LogType::progress,"ChannelsConfiguration object is created");
}

/**
 * @brief	ChannelsConfiguration destructor.
 */
ChannelsConfiguration::~ChannelsConfiguration() {
    consolelog("ChannelsConfiguration",LogType::progress,"ChannelsConfiguration object is deleted");
}

/**
 * @brief   It gets a channel input.
 * @param   index       channel index
 * @return  channel pointer
 */
ConfigurationChannel *ChannelsConfiguration::getChannel(int index) {
    return this->channels[index];
}

/**
 * @brief	It sets a number of channels up.
 * @param   number      number of channels
 */
void ChannelsConfiguration::setNumber(int number) {
    this->framework->findChild<QSpinBox *>("channels_number")->setValue(number);
    if (channels.size()<number) {
        for (int index = channels.size(); index < number;index++) {
            ConfigurationChannel *channel = new ConfigurationChannel(this->list, index);
            this->channels.push_back(channel);
            // User interface control
            QObject::connect(channel->label,SIGNAL(textChanged(QString)),this,SLOT(setLabel(QString)));
            QObject::connect(channel->levelslider,SIGNAL(valueChanged(int)),this,SLOT(setLevel(int)));
            QObject::connect(channel->activecheckbox,SIGNAL(clicked(bool)),this,SLOT(setActive(bool)));
            QObject::connect(channel->loadfile,SIGNAL(released()),this,SLOT(loadFile()));
            QObject::connect(channel->info,SIGNAL(released()),this,SLOT(openInfo()));
        }
    } else if (channels.size()>number) {
        for (int index = channels.size()-1; index >= number;index--) {
            this->deleteChannel(index);
            consolelog("ChannelsConfiguration",LogType::progress,"channel " + std::to_string(index) + " is deleted");
        }
    }
    consolelog("ChannelsConfiguration",LogType::interaction,"number of channels has been changed to " + std::to_string(number));
}

/**
 * @brief	It gets the number of channels.
 * @return  number of channels
 */
int ChannelsConfiguration::getNumber() {
    return this->framework->findChild<QSpinBox *>("channels_number")->value();
}

/**
 * @brief   It deletes a specific channel.
 * @param   index       channel index
 */
void ChannelsConfiguration::deleteChannel(int index) {
    delete this->framework->findChild<QGroupBox *>(QString("channels_%1").arg(index));
    this->channels.removeAt(index);
    consolelog("ChannelsConfiguration",LogType::progress,"Channels object is deleted");
}

/**
 * @brief   It gets the input channel index of a user interface element.
 * @param   element     user interface element
 * @return  index
 */
int ChannelsConfiguration::getChannelIndex(QObject *element) {
    std::string prefix = "channels_";
    return std::stoi(element->objectName().toStdString().substr(element->objectName().toStdString().find(prefix) + prefix.length(),1));
}

/**
 * @name    Inputs channels interface slots
 * @brief   User interface control functions of inputs channels.
 * @{
 */

/**
 * @brief   Input channel slots for setting the channel level.
 * @param   level
 */
void ChannelsConfiguration::setLevel(int level) {
    QObject::sender()->blockSignals(true);
    int index = this->getChannelIndex(QObject::sender());
    this->channels[index]->setLevel(level);
    consolelog("ChannelsConfiguration",LogType::interaction,"volume level of object " + std::to_string(index) + " set to " + std::to_string(level) + "%");
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Input channel slots for setting the channel label.
 * @param   label
 */
void ChannelsConfiguration::setLabel(QString label) {
    QObject::sender()->blockSignals(true);
    int index = this->getChannelIndex(QObject::sender());
    this->channels[index]->setLabel(label.toStdString());
    consolelog("ChannelsConfiguration",LogType::interaction,"label of channel " + std::to_string(index) + " has been changed to \"" + label.toStdString() + "\"");
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Input channel slots when active checkbox has been changed.
 * @param   state       current checkbox state
 */
void ChannelsConfiguration::setActive(bool state) {
    QObject::sender()->blockSignals(true);
    int index = this->getChannelIndex(QObject::sender());
    channels[index]->setActive(state);
    std::string message = "channel " + std::to_string(index) + " has been changed to ";
    if(state) {
        message += "active";
    } else {
        message += "inactive";
    }
    consolelog("ChannelsConfiguration",LogType::interaction,message);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Input channel slots for loading a file.
 */
void ChannelsConfiguration::loadFile() {
    QObject::sender()->blockSignals(true);
    int index = this->getChannelIndex(QObject::sender());
    consolelog("ChannelsConfiguration",LogType::interaction,"selecting source file for channel " + std::to_string(index));
    std::string filepath = QFileDialog::getOpenFileName(this->framework, "Load audio file","","*.wav").toStdString();
    if(filepath=="") {
        consolelog("ChannelsConfiguration",LogType::interaction,"canceling source file selection for channel " + std::to_string(index));
    } else {
        consolelog("ChannelsConfiguration",LogType::interaction,"selected \"" + filepath + "\" as source file for channel " + std::to_string(index));
        this->channels[index]->setFile(filepath);
    }
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Input channel slots for info action.
 */
void ChannelsConfiguration::openInfo() {
    QObject::sender()->blockSignals(true);
    int index = this->getChannelIndex(QObject::sender());
    AudioInfo info;
    info.setFile(this->channels[index]->file);
    info.setWindowTitle(this->channels[index]->label->text() + QString(" - Info"));
    info.exec();
    consolelog("ChannelsConfiguration",LogType::interaction,"channel " + std::to_string(index) + " is now showing info");
    QObject::sender()->blockSignals(false);
}

/** @} */

/**
 * @brief	Channel constructor.
 * @param   parent          channels user interface parent
 * @param   index           channel index
 */
ConfigurationChannel::ConfigurationChannel(QLayout *parent, int index) {
    // Elements creation
    this->groupbox = new QGroupBox();
    QGridLayout *layout = new QGridLayout(groupbox);
    this->label = new QLineEdit();
    this->levelslider = new QSlider(Qt::Horizontal);
    this->activecheckbox = new QCheckBox();
    this->loadfile = new QPushButton();
    this->info = new QPushButton();
    this->currentsource = new QLineEdit();
    // Initialization
    this->file = NULL;
    this->name = QString("Channel %1").arg(index).toStdString();
    this->audiostream = new AudioStream(ChannelsConfiguration::fs);
    this->setIndex(index);
    this->setLabel(this->name);
    this->setLevel(70);
    this->setActive(false);
    this->setFile("");
    this->info->setIcon(QIcon(QPixmap(":/icons/info")));
    // Elements attributes
    int layout_height = 150;          // height of channel configuration interface
    this->groupbox->setMinimumSize(200,0);
    this->groupbox->setMaximumSize(QWIDGETSIZE_MAX,layout_height);
    layout->setSpacing(8);
    this->label->setClearButtonEnabled(true);
    this->levelslider->setMinimum(0);
    this->levelslider->setMaximum(100);
    this->activecheckbox->setText("Active");
    this->loadfile->setText("Load file");
    this->info->setMaximumSize(20,20);
    this->currentsource->setMaximumHeight(15);
    this->currentsource->setReadOnly(true);
    // Layout
    layout->addWidget(this->label,0,0);
    layout->addWidget(this->levelslider,0,1);
    layout->addWidget(this->activecheckbox,1,0);
    layout->addWidget(this->loadfile,1,1);
    layout->addWidget(this->info,2,0);
    layout->addWidget(this->currentsource,2,1);
    layout->setMargin(10);
    parent->addWidget(this->groupbox);
    consolelog("ConfigurationChannel",LogType::progress,"Channel object is created");
}

/**
 * @brief	Channel desctructor.
 */
ConfigurationChannel::~ConfigurationChannel() {
}

/**
 * @brief   It gets the channel index.
 * @return  index
 */
int ConfigurationChannel::getIndex() {
    return this->index;
}

/**
 * @brief   It sets the channel index.
 * @param   index
 */
void ConfigurationChannel::setIndex(int index) {
    this->index = index;
    this->groupbox->setObjectName(QString("channels_%1").arg(index));
    QString prefix = groupbox->objectName() + "_";
    this->label->setObjectName(prefix + "objectname");
    this->levelslider->setObjectName(prefix + "level");
    this->activecheckbox->setObjectName(prefix + "active");
    this->loadfile->setObjectName(prefix + "loadfile");
    this->info->setObjectName(prefix + "info");
    this->currentsource->setObjectName(prefix + "currentsource");
}

/**
 * @brief   It sets the input object volume level.
 * @param   level           selected volume level
 */
void ConfigurationChannel::setLevel(int level) {
    this->level = (float)level/100;
    this->levelslider->setValue(level);
}

/**
 * @brief   It sets a label to the channel configuration name, i.e., group box title and label text.
 * @param   label
 */
void ConfigurationChannel::setLabel(std::string label) {
    this->groupbox->setTitle("#" + QString::number(this->index) + " " + QString::fromStdString(label));
    this->label->setText(QString::fromStdString(label));
}

/**
 * @brief   It sets if the channel is active.
 * @param   state
 */
void ConfigurationChannel::setActive(bool state) {
    this->active = state;
    this->activecheckbox->setChecked(state);
}

/**
 * @brief	It sets the source file.
 * @param   filepath        file path
 */
void ConfigurationChannel::setFile(std::string filepath) {
    if(filepath == "") {
        if(this->file->exists()) {
            delete this->file;
        }
        this->info->setEnabled(false);
    } else {
        this->file = new WAVFile(filepath);
        this->currentsource->setText(QString::fromStdString(filepath));
        this->info->setEnabled(true);
    }
}

/**
 * @brief   It reads data from the channel file.
 * @return  data
 */
float ConfigurationChannel::readData() {
    if(this->file->exists()) {
        float value = 0;
        quint16 data_16 = 0;
        quint32 data_32 = 0;
        quint32 amplitude = pow(2,this->file->header.bitspersample-1);
        int bytespersample = (int)(this->file->header.bitspersample / 8);
        switch (bytespersample) {
        case 2:
        default:
            data_16 = (qint16)this->file->readDataNumber(bytespersample,WAVFile::Endianess::littleendian);
            value = (float)data_16/(float)amplitude;
            break;
        case 4:
            data_32 = (qint32)this->file->readDataNumber(bytespersample,WAVFile::Endianess::littleendian);
            value = data_32/amplitude;
            break;
        }
        return value;
    } else {
        consolelog("ConfigurationChannel",LogType::error,"error while reading data because audio file does not exist");
        return 0;
    }
}

/**
 * @brief   It sends data to the audio channel associated to this input channel.
 * @param   data
 */
void ConfigurationChannel::sendData(float data) {
    data *= this->level;
    this->audiostream->push(data);
}
