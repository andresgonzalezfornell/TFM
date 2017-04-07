// Class libraries
#include "channels.h"
#include "ui_Coder.h"

/**
 * @brief	ChannelsConfiguration constructor.
 * @param   framework   user interface framework of channels configuration
 * @param   number      number of channels
 */
ChannelsConfiguration::ChannelsConfiguration(QWidget *framework, int number) {
    this->framework = framework;
    this->layout = framework->findChild<QLayout *>("channels_list_layout");
    this->setNumber(number);
    consolelog("Channels",LogType::progress,"ChannelsConfiguration object is created");
}

/**
 * @brief	ChannelsConfiguration destructor.
 */
ChannelsConfiguration::~ChannelsConfiguration() {
    consolelog("Channels",LogType::progress,"ChannelsConfiguration object is deleted");
}

/**
 * @brief   It gets a channel input.
 * @param   index       channel index
 * @return  channel
 */
Channel *ChannelsConfiguration::getChannel(int index) {
    return this->channels[index];
}

/**
 * @brief	It sets a number of channels up.
 * @param   number      number of channels
 */
void ChannelsConfiguration::setNumber(int number) {
    this->number = number;
    if (channels.size()<number) {
        for (int index = channels.size(); index < this->number;index++) {
            Channel *channel = new Channel(this->layout, index);
            this->channels.push_back(channel);
            // User interface control
            QObject::connect(channel->levelslider,SIGNAL(valueChanged(int)),this,SLOT(setLevel(int)));
            QObject::connect(channel->label,SIGNAL(textChanged(QString)),this,SLOT(setLabel(QString)));
            QObject::connect(channel->activecheckbox,SIGNAL(clicked(bool)),this,SLOT(setActive(bool)));
            QObject::connect(channel->loadfile,SIGNAL(released()),this,SLOT(loadFile()));
            QObject::connect(channel->playpause,SIGNAL(released()),this,SLOT(switchPlayPause()));
            QObject::connect(channel->previewbutton,SIGNAL(released()),this,SLOT(switchPreview()));
            QObject::connect(channel->info,SIGNAL(released()),this,SLOT(openInfo()));
        }
    } else if (channels.size()>number) {
        for (int index = channels.size()-1; index >= this->number;index--) {
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
    return this->number;
}

/**
 * @brief   It deletes a specific channel.
 * @param   index       channel index
 */
void ChannelsConfiguration::deleteChannel(int index) {
    delete this->framework->findChild<QGroupBox *>(QString("channels_%1").arg(index));
    this->channels.removeAt(index);
    consolelog("ChannelsInput",LogType::progress,"Channels object is deleted");
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
 * @brief	Input channel slots for play/pause action.
 */
void ChannelsConfiguration::switchPlayPause() {
    QObject::sender()->blockSignals(true);
    int index = this->getChannelIndex(QObject::sender());
    if (this->channels[index]->paused) {
        this->channels[index]->playFile();
        consolelog("ChannelsConfiguration",LogType::interaction,"channel " + std::to_string(index) + " has been resumed");
    } else {
        this->channels[index]->pauseFile();
        consolelog("ChannelsConfiguration",LogType::interaction,"channel " + std::to_string(index) + " has been paused");
    }
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Input channel slots for preview action.
 */
void ChannelsConfiguration::switchPreview() {
    QObject::sender()->blockSignals(true);
    int index = this->getChannelIndex(QObject::sender());
    if (this->channels[index]->preview) {
        this->channels[index]->mutePreview();
        consolelog("ChannelsConfiguration",LogType::interaction,"channel " + std::to_string(index) + " preview has been muted");
    } else {
        this->channels[index]->unmutePreview();
        consolelog("ChannelsConfiguration",LogType::interaction,"channel " + std::to_string(index) + " preview has been unmuted");
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
