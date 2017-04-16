#include "ChannelsList.h"

/**
 * @brief	ChannelsList constructor.
 * @param   framework   user interface framework of channels list
 * @param   number      number of channels
 */
ChannelsList::ChannelsList(QWidget *framework, int number) {
    this->framework = framework;
    this->prefix = framework->objectName().toStdString() + "_";
    this->layout = framework->findChild<QLayout *>(QString::fromStdString(prefix + "layout"));
    this->setChannelsNumber(number);
    consolelog("Channels",LogType::progress,"ChannelsList object is created");
}

/**
 * @brief	ChannelsList destructor.
 */
ChannelsList::~ChannelsList() {
    consolelog("Channels",LogType::progress,"ChannelsList object is deleted");
}

/**
 * @brief   It gets a channel.
 * @param   index       channel index
 * @return  channel pointer
 */
Channel *ChannelsList::getChannel(int index) {
    return this->channels[index];
}

/**
 * @brief   It deletes a channel.
 * @param   index       channel index
 */
void ChannelsList::deleteChannel(int index) {
    delete this->framework->findChild<QGroupBox *>(QString::fromStdString(this->prefix) + QString("channels_%1").arg(index));
    this->channels.removeAt(index);
    consolelog("ChannelsList",LogType::progress,"Channels object is deleted");
}

/**
 * @brief   It sets a number of channels up.
 * @param   number      number of channels
 */
void ChannelsList::setChannelsNumber(int number) {
    if (this->channels.size()<number) {
        for (int index = this->channels.size(); index < number;index++) {
            Channel *channel = new Channel(this->layout, this->prefix, index);
            // User interface control
            QObject::connect(channel->label,SIGNAL(textChanged(QString)),this,SLOT(setLabel(QString)));
            QObject::connect(channel->levelslider,SIGNAL(valueChanged(int)),this,SLOT(setLevel(int)));
            QObject::connect(channel->mutecheckbox,SIGNAL(clicked(bool)),this,SLOT(mute(bool)));
            QObject::connect(channel->bypasscheckbox,SIGNAL(clicked(bool)),this,SLOT(bypass(bool)));
            this->channels.push_back(channel);
            this->list.push_back(channel);
        }
    } else if (this->channels.size()>number) {
        for (int index = this->list.size()-1; index >= number;index--) {
            this->deleteChannel(index);
            consolelog("ChannelsList",LogType::progress,"channel " + std::to_string(index) + " is deleted");
        }
    }
    consolelog("ChannelsList",LogType::interaction,"number of channels has been changed to " + std::to_string(number));
}

/**
 * @brief   It gets the channel index of a user interface element.
 * @param   element     user interface element
 * @return  index
 */
int ChannelsList::getChannelIndex(QObject *element) {
    return std::stoi(element->objectName().toStdString().substr(element->objectName().toStdString().find(this->prefix) + this->prefix.length(),1));
}

/**
 * @name    Channels interface slots
 * @brief   User interface control functions of channels list.
 * @{
 */

/**
 * @brief   Slot for setting the channel label.
 * @param   label
 */
void ChannelsList::setLabel(QString label) {
    QObject::sender()->blockSignals(true);
    int index = this->getChannelIndex(QObject::sender());
    this->list[index]->setLabel(label.toStdString());
    consolelog("ChannelsList",LogType::interaction,"label of channel " + std::to_string(index) + " has been changed to \"" + label.toStdString() + "\"");
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slots for setting the channel level.
 * @param   level           from 0 to 100
 */
void ChannelsList::setLevel(int level) {
    QObject::sender()->blockSignals(true);
    int index = this->getChannelIndex(QObject::sender());
    this->list[index]->setLevel(level);
    consolelog("ChannelsList",LogType::interaction,"volume level of object " + std::to_string(index) + " set to " + std::to_string(level) + "%");
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slots when muted checkbox has been changed.
 * @param   state       current checkbox state
 */
void ChannelsList::mute(bool state) {
    QObject::sender()->blockSignals(true);
    int index = this->getChannelIndex(QObject::sender());
    this->list[index]->mute(state);
    std::string message = "channel " + std::to_string(index) + " has been changed to ";
    if(state) {
        message += "muted";
    } else {
        message += "unmuted";
    }
    consolelog("ChannelsList",LogType::interaction,message);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slots when muted checkbox has been changed.
 * @param   state       current checkbox state
 */
void ChannelsList::bypass(bool state) {
    QObject::sender()->blockSignals(true);
    int index = this->getChannelIndex(QObject::sender());
    channels[index]->bypass(state);
    std::string message = "channel " + std::to_string(index) + " has been changed to ";
    if(state) {
        message += "apply effects";
    } else {
        message += "bypass effects";
    }
    consolelog("ChannelsList",LogType::interaction,message);
    QObject::sender()->blockSignals(false);
}

/** @} */

/**
 * @brief	Channels constructor.
 * @param   framework       channel user interface framework
 * @param   prefix          prefix of objects name of channel user interface
 * @param   index           channel index
 */
Channel::Channel(QLayout *framework, std::string prefix, int index) {
    // Elements creation
    this->groupbox = new QGroupBox();
    QGridLayout *layout = new QGridLayout(groupbox);
    this->label = new QLineEdit();
    this->levelslider = new QSlider(Qt::Horizontal);
    this->mutecheckbox = new QCheckBox();
    this->volumeterwidget = new QWidget();
    this->bypasscheckbox = new QCheckBox();
    // Initialization
    this->name = QString("Channel %1").arg(index).toStdString();
    this->stream = new AudioStream(ChannelsList::fs);
    this->prefix = prefix;
    this->setIndex(index);
    this->setLabel(this->name);
    this->setLevel(70);
    this->volumeter = new Volumeter(this->volumeterwidget,ChannelsList::fs);
    // Elements attributes
    int layout_height = 150; // height of channel configuration interface
    this->groupbox->setMinimumSize(200,0);
    this->groupbox->setMaximumSize(QWIDGETSIZE_MAX,layout_height);
    layout->setSpacing(8);
    this->label->setClearButtonEnabled(true);
    this->levelslider->setMinimum(0);
    this->levelslider->setMaximum(100);
    this->mutecheckbox->setText("Muted");
    this->bypasscheckbox->setText("Bypassed");
    // Layout
    layout->addWidget(this->label,0,0);
    layout->addWidget(this->levelslider,1,0);
    layout->addWidget(this->mutecheckbox,1,1);
    layout->addWidget(this->bypasscheckbox,2,1);
    layout->addWidget(this->volumeterwidget,2,0);
    layout->setMargin(10);
    framework->addWidget(this->groupbox);
//    framework->addWidget(this->chart);
    consolelog("Channels",LogType::progress,"Channels object is created");
}

/**
 * @brief	Channels desctructor.
 */
Channel::~Channel() {
}

/**
 * @brief   It gets the channel index.
 * @return  index
 */
int Channel::getIndex() {
    return this->index;
}

void Channel::setIndex(int index) {
    this->index = index;
    this->groupbox->setObjectName(QString::fromStdString(this->prefix) + QString::number(index));
    this->label->setObjectName(QString::fromStdString(this->prefix) + QString::number(index) + "objectname");
    this->levelslider->setObjectName(QString::fromStdString(this->prefix) + QString::number(index) + "level");
    this->mutecheckbox->setObjectName(QString::fromStdString(this->prefix) + QString::number(index) + "mute");
    this->bypasscheckbox->setObjectName(QString::fromStdString(this->prefix) + QString::number(index) + "bypass");
    this->volumeterwidget->setObjectName(QString::fromStdString(this->prefix) + QString::number(index) + "volumeter");
}

/**
 * @brief   It sets a label to the channel name, i.e., group box title and label text.
 * @param   label
 */
void Channel::setLabel(std::string label) {
    this->groupbox->setTitle("#" + QString::number(this->index) + " " + QString::fromStdString(label));
    this->label->setText(QString::fromStdString(label));
}

/**
 * @brief   It mutes channel.
 * @param   state           true to mute the channel and false to unmuted
 */
void Channel::mute(bool state) {
    this->muted = state;
    this->mutecheckbox->setChecked(state);
}

/**
 * @brief   It sets channel to bypass effects.
 * @param   state           true to bypass effects and false to apply them
 */
void Channel::bypass(bool state) {
    this->bypassed = state;
    this->bypasscheckbox->setChecked(state);
}

/**
 * @brief   It sets the channel volume level.
 * @param   level           selected volume level
 */
void Channel::setLevel(int level) {
    this->level = (float)level/100;
    this->levelslider->setValue(level);
}

void Channel::setStream(AudioStream *stream) {

}
