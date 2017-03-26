// Class libraries
#include "device.h"
#include "ui_mainwindow.h"

/**
 * @brief	Device constructor.
 * @param   framework       user interface framework of device
 */
Device::Device(QWidget *framework) :
    deviceinfo(QAudioDeviceInfo::defaultInputDevice()),
    channel(0),
    volumeter(new DeviceLevel(framework->findChild<QWidget *>("device_level"))),
    audioinput(0)
{
    // Data channel
    initialize();
    // Ui
    QComboBox *device_selector = framework->findChild<QComboBox *>("device_selector");
    updateDevices(*device_selector);
    playPause(framework->findChild<QPushButton *>("device_pause"));
    consolelog("Device",LogType::progress,"Device object is created");
}

/**
 * @brief	Device destructor.
 */
Device::~Device() {
    channel->stop();
    audioinput->stop();
    consolelog("Device",LogType::progress,"Device object is deleted");
}

/**
 * @brief	It initializes audio device.
 */
void Device::initialize()
{
    consolelog("Device",LogType::progress,"initializing device");
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");
    QAudioDeviceInfo info(deviceinfo);
    if (!info.isFormatSupported(format)) {
        format = info.nearestFormat(format);
        consolelog("Device",LogType::warning,"default format not supported - trying to use nearest");
    }
    if (channel) {
        delete channel;
    }
    channel = new DeviceChannel(format, this);
    connect(this->channel, SIGNAL(newLevel(float)),this->volumeter,SLOT(setLevel(float)));
    audioinput = new QAudioInput(deviceinfo, format, this);
    channel->start();
    audioinput->start(channel);
}

/**
 * @brief	It updates system input devices combobox.
 * @param   device_selector    QComboBox object to update
 */
void Device::updateDevices(QComboBox &device_selector) {
    device_selector.blockSignals(true);
    for (int index = 0;index < device_selector.count();index++) {
        device_selector.removeItem(index);
    }
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    for(int index = 0;index < devices.count();index++) {
        device_selector.addItem(devices.value(index).deviceName());
        consolelog("Device",LogType::info,"Input device updated: " + devices.value(index).deviceName().toStdString());
    }
    device_selector.blockSignals(false);
}

/**
 * @brief	It gets selected input audio device.
 * @param   index           device index
 * @return  QAudioDeviceInfo selected device.
 */
QAudioDeviceInfo Device::getDevice(int index) {
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    return devices.value(index);
}

/**
 * @brief	It changes input audio device.
 * @param   index       Selected device index
 */
void Device::setDevice(int index) {
    channel->stop();
    audioinput->stop();
    audioinput->disconnect(this);
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    QAudioDeviceInfo deviceinfo = devices.value(index);
    consolelog("Device",LogType::progress,"input device changed to " + deviceinfo.deviceName().toStdString());
    initialize();
}

/**
 * @brief	Change input audio device.
 * @param   value       Selected value volume
 */
void Device::setVolume(int value)
{
        qreal volume = value/100;
        audioinput->setVolume(volume);
}

/**
 * @brief	Device play/pause action.
 * @param   button
 */
void Device::playPause(QPushButton *button) {
    QString text_old = button->text();
    QString text_new = "";
    if (text_old.toStdString()=="Play") {
        text_new = "Pause";
        this->audioinput->resume();
    } else if(text_old.toStdString()=="Pause") {
        text_new = "Play";
        this->audioinput->suspend();
    } else {
        text_new = "Pause";
        this->audioinput->resume();
    }
    button->setText(text_new);
    consolelog("Device",LogType::info,"device play/pause button is showing now: " + text_new.toStdString());
}

/**
 * @brief	Device push/pull mode action.
 * @param   button
 */
void Device::switchMode(QPushButton *button) {
    QString text_old = button->text();
    QString text_new = "";
    if (text_old.toStdString()=="Push") {
        text_new = "Pull";
    } else if(text_old.toStdString()=="Pull") {
        text_new = "Push";
    } else {
        text_new = "Pull";
    }
    button->setText(text_new);
    consolelog("Device",LogType::info,"device play/pause button is showing now: " + text_new.toStdString());
}
