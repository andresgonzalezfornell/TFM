#include "ChannelsList.h"
#include "ui_ChannelsCharts.h"

/**
 * @brief	ChannelsList constructor.
 * @param   framework       user interface framework of channels list
 * @param   number          number of channels
 * @param   showdevices     true to create device selector to send audio to the system audio output devices
 */
ChannelsList::ChannelsList(QWidget *framework, int number, bool showdevices) {
    this->prefix = framework->objectName().toStdString() + "_";
    this->framework = framework;
    this->layout = framework->findChild<QLayout *>(
                QString::fromStdString(prefix + "layout"));
    this->showdevices = showdevices;
    this->setSize(number);
    consolelog("Channels", LogType::progress, "ChannelsList object is created");
}

/**
 * @brief   ChannelsList destructor.
 */
ChannelsList::~ChannelsList() {
    consolelog("Channels", LogType::progress, "ChannelsList object is deleted");
}

/**
 * @brief   It gets a channel.
 * @param   index           channel index
 * @return  channel pointer
 */
Channel *ChannelsList::getChannel(int index) {
    return this->channels[index];
}

/**
 * @brief   It deletes a channel.
 * @param   index           channel index
 */
void ChannelsList::deleteChannel(int index) {
    delete this->framework->findChild<QGroupBox *>(
                QString::fromStdString(this->prefix)
                + QString("channels_%1").arg(index));
    this->channels.erase(this->channels.begin() + index);
    consolelog("ChannelsList", LogType::progress, "Channels object is deleted");
}

/**
 * @brief   It gets the number of channels.
 * @return  number of channels
 */
int ChannelsList::getSize() {
    return this->channels.size();
}

/**
 * @brief   It sets a number of channels up.
 * @param   size            number of channels
 */
void ChannelsList::setSize(int size) {
    if ((int) this->channels.size() < size) {
        for (int index = (int) this->channels.size(); index < size; index++) {
            Channel *channel = new Channel(this->layout, this->prefix, index,
                                           this->showdevices);
            QObject::connect(channel->label, SIGNAL(textChanged(QString)), this,
                             SLOT(setLabel(QString)));
            QObject::connect(channel->volumeslider,SIGNAL(valueChanged(int)),this,SLOT(setVolume(int)));
            QObject::connect(channel->mutecheckbox,SIGNAL(clicked(bool)),this,SLOT(mute(bool)));
            QObject::connect(channel->bypasscheckbox,SIGNAL(clicked(bool)),this,SLOT(bypass(bool)));
            this->channels.push_back(channel);
        }
    } else if ((int) this->channels.size() > size) {
        for (int index = (int) this->channels.size() - 1; index >= size;
             index--) {
            this->deleteChannel(index);
            consolelog("ChannelsList", LogType::progress,
                       "channel " + std::to_string(index) + " is deleted");
        }
    }
    consolelog("ChannelsList", LogType::interaction,
               "number of channels has been changed to " + std::to_string(size));
}

/**
 * @brief   It gets the channel index of a user interface element.
 * @param   element         user interface element
 * @return  index
 */
int ChannelsList::getIndex(QObject *element) {
    return std::stoi(
                element->objectName().toStdString().substr(
                    element->objectName().toStdString().find(this->prefix)
                    + this->prefix.length(), 1));
}

/**
 * @brief   It gets all channels names.
 * @return  list of channels names
 */
std::vector<std::string> ChannelsList::getNames() {
    std::vector<std::string> names;
    for (int index = 0; index < this->getSize(); index++) {
        names.push_back(this->channels[index]->name);
    }
    return names;
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
    int index = this->getIndex(QObject::sender());
    this->setLabel(label, index);
    emit namechanged(label, index);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slot for setting the channel label.
 * @param   label
 * @param   index           channel index
 */
void ChannelsList::setLabel(QString label, int index) {
    this->channels[index]->setLabel(label.toStdString());
    consolelog("ChannelsList", LogType::interaction,
               "label of channel " + std::to_string(index)
               + " has been changed to \"" + label.toStdString() + "\"");
}

/**
 * @brief   Slots for setting the channel level.
 * @param   	volume           integer number from 0 to 100
 */
void ChannelsList::setVolume(int volume) {
    QObject::sender()->blockSignals(true);
    int index = this->getIndex(QObject::sender());
    consolelog("ChannelsList", LogType::interaction,
               "changing volume level of object " + std::to_string(index) + " to "
               + std::to_string(volume) + "%");
    this->channels[index]->setVolume(volume);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slots when muted checkbox has been changed.
 * @param   state           current checkbox state
 */
void ChannelsList::mute(bool state) {
    QObject::sender()->blockSignals(true);
    int index = this->getIndex(QObject::sender());
    this->channels[index]->mute(state);
    std::string message = "channel " + std::to_string(index)
            + " has been changed to ";
    if (state) {
        message += "muted";
    } else {
        message += "unmuted";
    }
    consolelog("ChannelsList", LogType::interaction, message);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slots when muted checkbox has been changed.
 * @param   state           current checkbox state
 */
void ChannelsList::bypass(bool state) {
    QObject::sender()->blockSignals(true);
    int index = this->getIndex(QObject::sender());
    this->channels[index]->bypass(state);
    std::string message = "channel " + std::to_string(index)
            + " has been changed to ";
    if (state) {
        message += "apply effects";
    } else {
        message += "bypass effects";
    }
    consolelog("ChannelsList", LogType::interaction, message);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slots for setting an audio output device
 * @param   device           device index
 */
void ChannelsList::setDevice(int device) {
    QObject::sender()->blockSignals(true);
    int index = this->getIndex(QObject::sender());
    consolelog("ChannelsList", LogType::interaction,
               "changing audio output device of object " + std::to_string(index)
               + " to #" + std::to_string(device));
    //    this->channels[index]->audiooutput->setDevice(index);
    QObject::sender()->blockSignals(false);
}

/** @} */

/**
 * @brief	Channels constructor.
 * @param   framework       channel user interface framework
 * @param   prefix          prefix of objects name of channel user interface
 * @param   index           channel index
 * @param   isoutput     	true to create device selector to send audio to the system audio output devices
 */
Channel::Channel(QLayout *framework, std::string prefix, int index,
                 bool isoutput) {
    // Elements creation
    this->groupbox = new QGroupBox();
    QGridLayout *layout = new QGridLayout(groupbox);
    this->label = new QLineEdit();
    this->volumeslider = new QSlider(Qt::Horizontal);
    this->mutecheckbox = new QCheckBox();
    this->volumeterwidget = new QWidget();
    this->bypasscheckbox = new QCheckBox();
    this->deviceselector = new QComboBox();
    if (isoutput) {
        this->audiooutput = new AudioOutput(this->deviceselector,
                                            ChannelsList::fs, ChannelsList::samplesize);
    }
    // Initialization
    this->name = QString("Channel %1").arg(index).toStdString();
    this->prefix = prefix;
    this->isoutput = isoutput;
    this->setIndex(index);
    this->setLabel(this->name);
    this->volumeter = new Volumeter(this->volumeterwidget, ChannelsList::fs);
    this->mute(false);
    this->bypass(false);
    this->setVolume(100);
    // Elements attributes
    int layout_height = 150; // height of channel configuration interface
    this->groupbox->setMinimumSize(200, 0);
    this->groupbox->setMaximumSize(QWIDGETSIZE_MAX, layout_height);
    layout->setSpacing(8);
    this->label->setClearButtonEnabled(true);
    this->volumeslider->setMinimum(0);
    this->volumeslider->setMaximum(100);
    this->mutecheckbox->setText("Muted");
    this->bypasscheckbox->setText("Bypassed");
    this->bypasscheckbox->setEnabled(!isoutput);
    // Layout
    layout->addWidget(this->label, 0, 0);
    layout->addWidget(this->volumeslider, 1, 0);
    layout->addWidget(this->mutecheckbox, 1, 1);
    layout->addWidget(this->bypasscheckbox, 2, 1);
    layout->addWidget(this->volumeterwidget, 2, 0);
    if (isoutput) {
        layout->addWidget(this->deviceselector, 3, 0, 2, 0, Qt::AlignLeft);
    }
    layout->setMargin(10);
    framework->addWidget(this->groupbox);
    consolelog("Channels", LogType::progress, "Channels object is created");
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
    this->groupbox->setObjectName(
                QString::fromStdString(this->prefix) + QString::number(index));
    this->label->setObjectName(
                QString::fromStdString(this->prefix) + QString::number(index)
                + "objectname");
    this->volumeslider->setObjectName(
                QString::fromStdString(this->prefix) + QString::number(index)
                + "level");
    this->mutecheckbox->setObjectName(
                QString::fromStdString(this->prefix) + QString::number(index)
                + "mute");
    this->bypasscheckbox->setObjectName(
                QString::fromStdString(this->prefix) + QString::number(index)
                + "bypass");
    this->volumeterwidget->setObjectName(
                QString::fromStdString(this->prefix) + QString::number(index)
                + "volumeter");
}

/**
 * @brief   It sets a label to the channel name, i.e., group box title and label text.
 * @param   label
 */
void Channel::setLabel(std::string label) {
    this->name = label;
    this->groupbox->setTitle(
                "#" + QString::number(this->index) + " "
                + QString::fromStdString(label));
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
 * @param   volume          integer number from 0 to 100
 */
void Channel::setVolume(int volume) {
    this->volume = (double) volume / 100;
    if (this->isoutput) {
        this->audiooutput->setVolume(this->volume);
    }
    this->volumeslider->setValue(volume);
}

/**
 * @brief   ChannelsCharts constructor.
 * @param   parent          user inteface parent object
 * @param   input           input signal pointer
 * @param   output          output signal pointer
 * @param   input_channels  input channels object
 * @param   output_channels output channels object
 * @param   samples         number of samples each channel
 * @param   fs              audio signal sampling [Hz]
 */
ChannelsCharts::ChannelsCharts(float **input, float **output, ChannelsList *input_channels, ChannelsList *output_channels, int samples, int fs, QWidget *parent) : QDialog(parent), ui(new Ui::ChannelsCharts) {
    ui->setupUi(this);
    this->fs = fs;
    this->input = input;
    this->output = output;
    this->input_channels = input_channels;
    this->output_channels = output_channels;
    this->samples = samples;
    this->input_chart = new Chart2D(ui->input_chart);
    this->output_chart = new Chart2D(ui->output_chart);
    // Initialization
    this->setTimeCursor(0);
    this->setScope(100);
    ui->scope->setMinimum(10);
    ui->scope->setMaximum(std::floor(this->samples * 1000 / this->fs));
    this->updateSelectors();
    QObject::connect(ui->cursor, SIGNAL(valueChanged(int)), this, SLOT(setTimeCursor(int)));
    QObject::connect(ui->cursor, SIGNAL(valueChanged(int)), this, SLOT(plot()));
    QObject::connect(ui->scope, SIGNAL(valueChanged(int)), this, SLOT(setScope(int)));
    QObject::connect(ui->scope, SIGNAL(valueChanged(int)), this, SLOT(plot()));
    this->plot();
}

/**
 * @brief   ChannelsCharts destructor.
 */
ChannelsCharts::~ChannelsCharts() {
    consolelog("ChannelsCharts", LogType::progress, "ChannelsCharts object is deleted");
}

/**
 * @brief   It sets the start time cursor.
 * @param   sample          first sample of the chart
 */
void ChannelsCharts::setTimeCursor(int sample) {
    QObject::blockSignals(true);
    double duration = (double) sample / this->fs;
    int h = (int) std::floor(duration / 60 / 60);
    int m = (int) std::floor(duration / 60) % 60;
    int s = (int) std::floor(duration) % 60;
    int ms = (int) std::round(duration * 1000) % 1000;
    std::string text = "";
    if (h < 10) {
        text += std::to_string(0);
    }
    text += std::to_string(h) + ":";
    if (m < 10) {
        text += std::to_string(0);
    }
    text += std::to_string(m) + ":";
    if (s < 10) {
        text += std::to_string(0);
    }
    text += std::to_string(s) + ".";
    if (ms < 100) {
        text += std::to_string(0);
        if (ms < 10) {
            text += std::to_string(0);
        }
    }
    text += std::to_string(ms);
    ui->cursor->setValue(sample);
    ui->timer->setText(QString::fromStdString(text));
    consolelog("ChannelsCharts", LogType::interaction, "start time cursor set to sample " + std::to_string(sample) + " (" + text + ")");
    QObject::blockSignals(false);
}

/**
 * @brief   It gets the start time cursor.
 * @return  first sample of the chart
 */
int ChannelsCharts::getTimeCursor() {
    return ui->cursor->value();
}

/**
 * @brief   It sets the charts scope.
 * @param   time            charts scope [ms]
 */
void ChannelsCharts::setScope(int time) {
    QObject::blockSignals(true);
    ui->scope->setValue(time);
    int remainder = this->samples - std::floor((double) time / 1000 * this->fs);
    if (remainder > 0) {
        ui->cursor->setEnabled(true);
        ui->cursor->setMaximum(remainder);
    } else {
        ui->cursor->setEnabled(false);
    }
    consolelog("ChannelsCharts", LogType::interaction, "scope time set to " + std::to_string(time) + " ms");
    QObject::blockSignals(false);
}

/**
 * @brief   It gets the charts scope.
 * @return  number of sample of charts scope
 */
int ChannelsCharts::getScope() {
    return std::floor(ui->scope->value() * this->fs / 1000);
}

/**
 * @brief   It loads current channels names on chart selectors.
 */
void ChannelsCharts::updateSelectors() {
    QObject::disconnect(ui->input_selector, SIGNAL(currentIndexChanged(int)), this, SLOT(plot()));
    QObject::disconnect(ui->output_selector, SIGNAL(currentIndexChanged(int)), this, SLOT(plot()));
    ui->input_selector->clear();
    ui->output_selector->clear();
    std::vector<std::string> input_names = this->input_channels->getNames();
    std::vector<std::string> output_names = this->output_channels->getNames();
    for (int index = 0; index < this->input_channels->getSize(); index++) {
        ui->input_selector->addItem(
                    QString::fromStdString(input_names[index] + " - Signal"));
        ui->input_selector->addItem(
                    QString::fromStdString(input_names[index] + " - Spectrum"));
    }
    for (int index = 0; index < this->input_channels->getSize(); index++) {
        ui->output_selector->addItem(
                    QString::fromStdString(output_names[index] + " - Signal"));
        ui->output_selector->addItem(
                    QString::fromStdString(output_names[index] + " - Spectrum"));
    }
    QObject::connect(ui->input_selector, SIGNAL(currentIndexChanged(int)), this, SLOT(plot()));
    QObject::connect(ui->output_selector, SIGNAL(currentIndexChanged(int)), this, SLOT(plot()));
}


/**
 * @brief   It plots the current input and ouput signals.
 */
void ChannelsCharts::plot() {
    QVector < QPointF > points;
    float *samples;
    Chart2D *chart;
    int remainder = 0;
    double range[2][2];
    for (int plot = 0; plot < 2; plot++) {
        switch (plot) {
        case 0:
            // Input
            samples = this->input[(int)std::floor(ui->input_selector->currentIndex() / 2)];
            remainder = (int)ui->input_selector->currentIndex() % 2;
            chart = this->input_chart;
            break;
        case 1:
            // Output
            samples = this->output[(int)std::floor(ui->output_selector->currentIndex() / 2)];
            remainder = (int)ui->output_selector->currentIndex() % 2;
            chart = this->output_chart;
            break;
        }
        switch (remainder) {
        case 0:
            // Signal plot
            range[0][0] = (double) this->getTimeCursor() / this->fs;
            range[0][1] = (double) (this->getTimeCursor() + this->getScope() - 1) / this->fs ;
            range[1][0] = -1;
            range[1][1] = 1;
            chart->setOptions(Chart2D::ChartOptions::labelX);
            for (int sample = this->getTimeCursor(); sample < this->getTimeCursor() + this->getScope(); sample++) {
                points.push_back(QPointF((float) sample / this->fs, samples[sample]));
            }
            break;
        case 1:
            // Spectrum plot
            std::vector<float> vector = std::vector<float>(samples + this->getTimeCursor(), samples + this->getTimeCursor() + this->getScope());;
            AudioSignal signal = AudioSignal(vector, this->fs);
            range[0][0] = 0;
            range[0][1] = (double) this->fs / 2;
            range[1][0] = 0;
            range[1][1] = 1;
            chart->setOptions(Chart2D::ChartOptions::labelX);
            std::vector<float> spectrum = signal.getSpectrum();
            for (int frequency = 0; frequency < std::ceil((double) this->getScope() / 2); frequency++) {
                points.push_back(QPointF((float) frequency * this->fs / this->getScope(), spectrum[frequency]));
            }
            break;
        }
        chart->clear();
        chart->setRange(range);
        chart->setPoints(points);
        points.clear();
    }
}
