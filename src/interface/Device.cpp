// Class libraries
#include "device.h"
#include "ui_mainwindow.h"

/**
 * @brief	Device constructor.
 * @param   framework       user interface framework of device
 */
Device::Device(QWidget *framework, float fs) :
    deviceinfo(QAudioDeviceInfo::defaultInputDevice()),
    channel(0),
    volumeter(new DeviceLevel(framework->findChild<QWidget *>("device_level"))),
    audioinput(0)
{
    this->fs = fs;
    this->framework = framework;
    initialize();
    QComboBox *device_selector = this->framework->findChild<QComboBox *>("device_selector");
    updateDevices(*device_selector);
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
    this->format.setSampleRate(this->fs);
    this->format.setChannelCount(1);
    this->format.setSampleSize(16);
    this->format.setSampleType(QAudioFormat::SignedInt);
    this->format.setByteOrder(QAudioFormat::LittleEndian);
    this->format.setCodec("audio/pcm");
    QAudioDeviceInfo info(deviceinfo);
    if (!info.isFormatSupported(format)) {
        this->format = info.nearestFormat(format);
        consolelog("Device",LogType::warning,"default format not supported - trying to use nearest");
    }
    channel = new DeviceChannel(this->format, this);
    QObject::connect(this->channel, SIGNAL(newData(float)),this,SLOT(sendData(float)));
    QObject::connect(this->channel, SIGNAL(newLevel(float)),this->volumeter,SLOT(setLevel(float)));
    this->audioinput = new QAudioInput(this->deviceinfo, this->format, this);
    this->channel->start();
    this->audioinput->start(channel);
    this->mute();
}

/**
 * @brief   It sends new data from the device channel to Objects.
 * @param   data
 */
void Device::sendData(float data) {
    emit newData(data);
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
    //audioinput->disconnect(this);
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    this->deviceinfo = devices.value(index);
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
 * @brief	It sets device as playing.
 */
void Device::unmute() {
    this->channel->unmute();
    QPushButton *button = this->framework->findChild<QPushButton *>("device_muting");
    button->setText("Mute");
}

/**
 * @brief	It sets device as paused.
 */
void Device::mute() {
    this->channel->mute();
    QPushButton *button = this->framework->findChild<QPushButton *>("device_muting");
    button->setText("Unmute");
}

/**
 * @brief	Device play/pause action.
 */
void Device::switchMuting() {
    QPushButton *button = this->framework->findChild<QPushButton *>("device_muting");
    if (button->text().toStdString()=="Unmute") {
        this->unmute();
    } else if(button->text().toStdString()=="Mute") {
        this->mute();
    } else {
        this->unmute();
    }
}
