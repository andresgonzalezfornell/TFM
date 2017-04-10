#include "ChannelsList.h"

/**
 * @brief	ChannelsList constructor.
 * @param   framework   user interface framework of channels list
 * @param   number      number of channels
 */
ChannelsList::ChannelsList(QWidget *framework, int number, std::string prefix) {
    this->framework = framework;
    this->prefix = prefix;
    this->list = framework->findChild<QLayout *>("channels_list_layout");
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

void ChannelsList::addChannel(Channel *channel) {

}

void ChannelsList::deleteChannel(int index) {

}

void ChannelsList::setChannelsNumber(int number) {

}

/**
 * @brief	Channels constructor.
 * @param   parent          channels user interface parent
 * @param   index           channel index
 */
Channel::Channel(QLayout *parent, int index, std::string prefix) {
    // Elements creation
    this->groupbox = new QGroupBox();
    QGridLayout *layout = new QGridLayout(groupbox);
    this->label = new QLineEdit();
    this->selectedcheckbox = new QCheckBox();
    this->levelslider = new QSlider(Qt::Horizontal);
    this->volumeterwidget = new QWidget();
    // Initialization
    this->prefix = prefix;
    this->name = QString("Channel %1").arg(index).toStdString();
    this->stream = new AudioStream(ChannelsList::fs);
    this->setIndex(index);
    this->setLabel(this->name);
    this->setSelected(false);
    this->setLevel(70);
    this->volumeter = new Volumeter(this->volumeterwidget,ChannelsList::fs);
    // Elements attributes
    int layout_height = 150;          // height of channel configuration interface
    this->groupbox->setMinimumSize(200,0);
    this->groupbox->setMaximumSize(QWIDGETSIZE_MAX,layout_height);
    layout->setSpacing(8);
    this->label->setClearButtonEnabled(true);
    this->selectedcheckbox->setText("Selected");
    this->levelslider->setMinimum(0);
    this->levelslider->setMaximum(100);
    // Layout
    layout->addWidget(this->label,0,0);
    layout->addWidget(this->selectedcheckbox,0,1);
    layout->addWidget(this->levelslider,1,0);
    layout->addWidget(this->volumeterwidget,1,1);
    layout->setMargin(10);
    parent->addWidget(this->groupbox);
    parent->addWidget(this->chart);
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
    this->selectedcheckbox->setObjectName(QString::fromStdString(this->prefix) + QString::number(index) + "selected");
    this->levelslider->setObjectName(QString::fromStdString(this->prefix) + QString::number(index) + "level");
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

void Channel::setSelected(bool state) {

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
