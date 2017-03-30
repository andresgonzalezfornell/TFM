// Class libraries
#include "device.h"
#include "ui_mainwindow.h"

/**
 * @brief	Device constructor.
 * @param   framework       user interface framework of device
 * @param   fs              sampling frequency
 */
Device::Device(QWidget *framework, float fs) :
    deviceinfo(QAudioDeviceInfo::defaultInputDevice()),
    channel(0),
    volumeter(new Volumeter(framework->findChild<QWidget *>("device_volumeter"))),
    audioinput(0)
{
    this->fs = fs;
    this->framework = framework;
    initialize();
    QComboBox *device_selector = this->framework->findChild<QComboBox *>("device_selector");
    updateDevices(*device_selector);
    // User interface slots
    QObject::connect(this->framework->findChild<QPushButton *>("device_muting"),SIGNAL(released()),this,SLOT(switchMuting()));
    QObject::connect(this->framework->findChild<QComboBox *>("device_selector"),SIGNAL(currentIndexChanged(int)),this,SLOT(setDevice(int)));
    QObject::connect(this->framework->findChild<QSlider *>("device_level"),SIGNAL(valueChanged(int)),this,SLOT(setLevel(int)));
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
 * @brief   It sends new data from the device channel to Objects.
 * @param   data
 */
void Device::sendData(float data) {
    emit newData(data);
}

/**
 * @name    Device interface slots
 * @brief   User interface control functions of device.
 * @{
 */

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
    consolelog("Device",LogType::interaction,"input device changed to " + deviceinfo.deviceName().toStdString());
    initialize();
}

/**
 * @brief	Device mute/unmute action.
 */
void Device::switchMuting() {
    QPushButton *button = this->framework->findChild<QPushButton *>("device_muting");
    if (button->text().toStdString()=="Unmute") {
        this->unmute();
        consolelog("Device",LogType::interaction,"device has been unmuted");
    } else if(button->text().toStdString()=="Mute") {
        this->mute();
        consolelog("Device",LogType::interaction,"device has been muted");
    } else {
        this->unmute();
        consolelog("Device",LogType::interaction,"device has been unmuted");
    }
}

/**
 * @brief	Change input audio device.
 * @param   value       Selected value volume
 */
void Device::setLevel(int value)
{
        float volume = (float)value/100;
        audioinput->setVolume(volume);
        consolelog("Device",LogType::interaction,"device volumen level set to " + std::to_string(value) + "%");
}

/** @} */
