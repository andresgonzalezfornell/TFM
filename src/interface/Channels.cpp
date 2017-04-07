// System libraries
#include "math.h"
#include "stdlib.h"
// Class libraries
#include "channels.h"
#include "Volumeter.h"
#include "ui_Coder.h"

/**
 * @brief	Channels constructor.
 * @param   framework   user interface framework of channels
 */
Channels::Channels(QWidget *framework) {
    framework->setUpdatesEnabled(true);
    this->master = new AudioSignal(Channels::fs);
    float chartrange[2][2] = {{0,(float)Channels::fs/2},{0,1e-3}};
    this->audiochart = new AudioChart(framework->findChild<QWidget *>("channels_chart"),chartrange,"",AudioChart::ChartOptions::labelX);
    this->channelsconfiguration = new ChannelsConfiguration(framework->findChild<QWidget *>("channels_list"),framework->findChild<QSpinBox *>("channels_number")->value());
    // User interface slots
    QObject::connect(framework->findChild<QSpinBox *>("channels_number"),SIGNAL(valueChanged(int)),this->channelsconfiguration,SLOT(setNumber(int)));
    consolelog("Channels",LogType::progress,"Channels object is created");
}

/**
 * @brief	Channels destructor.
 */
Channels::~Channels() {
    consolelog("Channels",LogType::progress,"Channels object is deleted");
}

/**
 * @brief   It sets the signal sampling frequency
 * @param   fs          signal sampling frequency [Hz]
 */
void Channels::setfs(int fs) {
    Channels::fs = fs;
    if(this != NULL) {
        this->master = new AudioSignal(fs);
        float chartrange[2][2] = {{0,(float)master->fs/2},{0,1e-3}};
        audiochart->setRange(chartrange);
        consolelog("Channels",LogType::progress,"channels signal sampling frequency has been changed to " + std::to_string(fs));
    }
}
