#include "AudioOutput.h"
#include "ui_AudioTest.h"

const int buffersize = 32768;

/**
 * @brief   AudioOuput constructor.
 * @param   selector            user interface combo box to select audio device
 * @param   fs                  signal sampling frequency
 * @param   samplesize          audio sample size [bits]
 */
AudioOutput::AudioOutput(QComboBox *selector, int fs, int samplesize) {
    this->fs = fs;
    this->samplesize = samplesize;
    this->device = QAudioDeviceInfo::defaultOutputDevice();
    this->outputdevice = NULL;
    this->buffer = QByteArray(buffersize, 0);
    this->selector = selector;
    QObject::connect(this->selector,SIGNAL(currentIndexChanged(int)),this,SLOT(setDevice(int)));
    this->setDevices();
    consolelog("AudioOutput",LogType::progress,"AudioOutput object is created");
}

/**
 * @brief   AudioOutput destructor.
 */
AudioOutput::~AudioOutput() {
    consolelog("AudioOutput",LogType::progress,"AudioOutput object is deleted");
}

/**
 * @brief   It initializes output audio device.
 */
void AudioOutput::initialize() {
    if(this->outputdevice) {
        this->stop();
    }
    this->format = QAudioFormat();
    this->format.setSampleRate(this->fs);
    this->format.setChannelCount(1);
    this->format.setSampleSize(16);
    this->format.setCodec("audio/pcm");
    this->format.setByteOrder(QAudioFormat::LittleEndian);
    this->format.setSampleType(QAudioFormat::SignedInt);
    QAudioDeviceInfo info = QAudioDeviceInfo(this->device);
    if (!info.isFormatSupported(format)) {
        consolelog("AudioOutput",LogType::warning,"default format not supported - trying to use nearest");
        format = info.nearestFormat(format);
    }
    this->outputdevice = new OutputDevice(format);
    this->audiooutput = new QAudioOutput(device, format, this);
    this->start();
}

/**
 * @brief   It resumes audio output playback.
 */
void AudioOutput::start() {
    this->outputdevice->open(QIODevice::ReadOnly);
    this->audiooutput->start(this->outputdevice);
    consolelog("AudioOutput",LogType::progress,"audio output device has been started playing through " + device.deviceName().toStdString());
}

/**
 * @brief   It stops audio output playback.
 */
void AudioOutput::stop() {
    this->outputdevice->cursor = 0;
    this->outputdevice->close();
    this->audiooutput->stop();
    this->audiooutput->disconnect(this);
    consolelog("AudioOutput",LogType::progress,"audio output device has been stopped playing through " + device.deviceName().toStdString());
}

/**
 * @brief   It sets all available audio output devices.
 */
void AudioOutput::setDevices() {
    this->setDevices(QAudioDeviceInfo::availableDevices(QAudio::AudioOutput));
}

/**
 * @brief   It sets a list of audio devices.
 * @param   devices
 */
void AudioOutput::setDevices(QList<QAudioDeviceInfo> devices) {
    this->devices = devices;
    QStringList list = QStringList();
    for (int index = 0; index < devices.size();index++) {
        list.push_back(this->devices[index].deviceName());
        if (this->devices[index] == QAudioDeviceInfo::defaultOutputDevice()) {
            this->setDevice(index);
        }
        consolelog("AudioOutput",LogType::info,"Output device #" + std::to_string(index) + " : " + devices[index].deviceName().toStdString());
    }
    QObject::disconnect(this->selector,SIGNAL(currentIndexChanged(int)),this,SLOT(setDevice(int)));
    this->selector->clear();
    this->selector->addItems(list);
    QObject::connect(this->selector,SIGNAL(currentIndexChanged(int)),this,SLOT(setDevice(int)));
}

/**
 * @brief   It sets audio output volume level.
 * @param   volume              real number from 0 to 1
 */
void AudioOutput::setVolume(float volume) {
    this->volume = volume;
    this->audiooutput->setVolume(qreal(volume));
}

/**
 * @name    User interface slots
 * @brief   User interface control functions of AudioOutput.
 * @{
 */

/**
 * @brief   It selects an output device.
 * @param   index               device index
 */
void AudioOutput::setDevice(int index) {
    if (this->outputdevice) {
        this->stop();
    }
    this->device = this->devices[index];
    this->selector->setCurrentIndex(index);
    this->initialize();
    consolelog("AudioOutput",LogType::interaction,"selected output device set to #" + std::to_string(index) + " : " + this->devices[index].deviceName().toStdString());
}

/** @} */

/**
 * @brief   OutputDevice constructor
 * @param   format          audio format object
 */
OutputDevice::OutputDevice(const QAudioFormat format) : QIODevice() {
    this->cursor = 0;
    if (format.isValid()) {
        this->format = format;
        // setting amplitude and offset parameters
        int sign = 0;
        switch (format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            sign = 0;
            this->offset = 1;
            break;
        case QAudioFormat::SignedInt:
        default:
            sign = 1;
            this->offset = 0;
            break;
        }
        this->amplitude = std::pow(2,format.sampleSize()-sign);
        consolelog("OutputDevice",LogType::progress,"OutputDevice object is created");
    } else {
        consolelog("OutputDevice",LogType::error,"out audio format is not valid");
    }
}

/**
 * @brief   OutputDevice destructor.
 */
OutputDevice::~OutputDevice() {
    consolelog("OutputDevice",LogType::progress,"OutputDevice object is deleted");
}

/**
 * @brief   It sends an audio signal to the buffer to be sent to the audio output device.
 * @param   signal
 */
void OutputDevice::send(AudioSignal signal) {
    quint8 data8;
    quint16 data16;
    quint32 data32;
    int channels = this->format.channelCount();
    int sample_bytes = this->format.sampleSize() / 8;
    long int samples = signal.size;
    long int length = samples * channels * sample_bytes;
    buffer.resize(length);
    unsigned char *datapointer = reinterpret_cast<unsigned char *>(buffer.data());
    double value;
    for (int sample = 0; sample < samples; sample++) {
        value = (offset + signal.getSample(sample))*amplitude;
        if (this->format.sampleType() == QAudioFormat::UnSignedInt) {
            value /= 2;
        }
        for (int channel=0; channel<channels; ++channel) {
            switch (this->format.sampleSize()) {
            case 8:
                switch (this->format.sampleType()) {
                case QAudioFormat::UnSignedInt:
                    data8 = static_cast<quint8>(value);
                    break;
                case QAudioFormat::SignedInt:
                default:
                    data8 = static_cast<qint8>(value);
                    break;
                }
                *reinterpret_cast<quint8*>(datapointer) = data8;
                break;
            case 16:
            default:
                switch (this->format.sampleType()) {
                case QAudioFormat::UnSignedInt:
                    data16 = static_cast<quint16>(value);
                    break;
                case QAudioFormat::SignedInt:
                default:
                    data16 = static_cast<qint16>(value);
                    break;
                }
                switch (this->format.byteOrder()) {
                case QAudioFormat::LittleEndian:
                    qToLittleEndian<quint16>(data16, datapointer);
                    break;
                case QAudioFormat::BigEndian:
                default:
                    qToBigEndian<quint16>(data16, datapointer);
                    break;
                }
                break;
            case 32:
                switch (this->format.sampleType()) {
                case QAudioFormat::UnSignedInt:
                    data32 = static_cast<quint32>(value);
                    break;
                case QAudioFormat::SignedInt:
                default:
                    data32 = static_cast<qint32>(value);
                    break;
                }
                switch (this->format.byteOrder()) {
                case QAudioFormat::LittleEndian:
                    qToLittleEndian<quint32>(data32, datapointer);
                    break;
                case QAudioFormat::BigEndian:
                default:
                    qToBigEndian<quint32>(data32, datapointer);
                    break;
                }
                break;
            }
            datapointer += sample_bytes;
        }
    }
}

/**
 * @brief   It plays an audio test by generating a tone.
 * @param   amplitude           tone amplitude (from 0 to 1)
 * @param   frequency           tone frequency [Hz]
 * @param   duration            test duration [s]
 */
void OutputDevice::test(double amplitude, double frequency, float duration) {
    int fs = this->format.sampleRate();
    long int N = std::floor(fs * duration);
    AudioSignal x = AudioSignal(fs);
    for (int n = 0; n < N; n++) {
        x.addSample(std::sin(2*M_PI*frequency/fs*n)*amplitude);
    }
    this->send(x);
}

/**
 * @brief   It gets data from the audio output device.
 * @param   data                data pointer
 * @param   length              data length
 * @return
 */
qint64 OutputDevice::readData(char *data, qint64 length) {
    qint64 total = 0;
    if (!buffer.isEmpty()) {
        while (length - total > 0) {
            const qint64 chunk = qMin((buffer.size() - cursor), length - total);
            memcpy(data + total, buffer.constData() + cursor, chunk);
            cursor = (cursor + chunk) % buffer.size();
            total += chunk;
        }
    }
    return total;
}

/**
 * @brief   It gets written data from the audio input device (not used).
 * @param   data                data pointer
 * @param   length              data length
 * @return
 */
qint64 OutputDevice::writeData(const char *data, qint64 length) {
    Q_UNUSED(data);
    Q_UNUSED(length);
    return 0;
}

/**
 * @brief   It gets available bytes to be read by the audio output device.
 * @return
 */
qint64 OutputDevice::bytesAvailable() const {
    return buffer.size() + QIODevice::bytesAvailable();
}

/**
 * @brief   AudioTest constructor.
 * @param   parent              window parent
 */
AudioTest::AudioTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AudioTest),
    audiooutput(0)
{
    ui->setupUi(this);
    QStringList sampleformats = QStringList();
    sampleformats << "8 bits" << "16 bits" << "32 bits";
    ui->sampleformat->addItems(sampleformats);
    QStringList fs = QStringList();
    fs << "44100 Hz";
    ui->fs->addItems(fs);
    // Default values
    ui->sampleformat->setCurrentIndex(2);
    ui->fs->setCurrentIndex(0);
    ui->frequency->setValue(440);
    ui->amplitude->setValue(50);
    QObject::connect(ui->device,SIGNAL(currentIndexChanged(int)),this,SLOT(setDevice(int)));
    QObject::connect(ui->sampleformat,SIGNAL(currentIndexChanged(int)),this,SLOT(setSampleFormat(int)));
    QObject::connect(ui->fs,SIGNAL(currentIndexChanged(int)),this,SLOT(setFS(int)));
    QObject::connect(ui->frequency,SIGNAL(valueChanged(double)),this,SLOT(setFrequency(double)));
    QObject::connect(ui->amplitude,SIGNAL(valueChanged(int)),this,SLOT(setAmplitude(int)));
    consolelog("AudioTest",LogType::progress,"AudioTest object is created");
    this->start();
}

/**
 * @brief   AudioTest destructor
 */
AudioTest::~AudioTest() {
    if (this->audiooutput) {
        delete this->audiooutput;
    }
    consolelog("AudioTest",LogType::progress,"AudioTest object is deleted");
}

/**
 * @name    User interface slots
 * @brief   They are called when a user interface element is being changed.
 * @{
 */

/**
 * @brief   It sets a device
 * @param   index               device index from the combo box
 */
void AudioTest::setDevice(int index) {
    consolelog("AudioTest",LogType::interaction,"device set to #" + std::to_string(index) + " : " + ui->device->currentText().toStdString());
    this->play();
}

/**
 * @brief   It sets a sample format
 * @param   index               sample format index from the combo box
 */
void AudioTest::setSampleFormat(int index) {
    consolelog("AudioTest",LogType::interaction,"sample format set to #" + std::to_string(index) + " : " + ui->sampleformat->currentText().toStdString());
    this->start();
}

/**
 * @brief   It sets a sampling frequency
 */
void AudioTest::setFS(int index) {
    consolelog("AudioTest",LogType::interaction,"sampling frequency set to #" + std::to_string(index) + " = " + ui->fs->currentText().toStdString());
    this->start();
}

/**
 * @brief   It sets an amplitude
 */
void AudioTest::setAmplitude(int amplitude) {
    consolelog("AudioTest",LogType::interaction,"amplitude set to " + std::to_string((float)amplitude/100));
    this->start();
}

/**
 * @brief   It sets a tone frequency
 */
void AudioTest::setFrequency(double frequency) {
    consolelog("AudioTest",LogType::interaction,"tone frequency set to " + std::to_string(frequency) + " Hz");
    this->start();
}

/** @} */

/**
 * @brief   It gets the sample size from the selected sample format
 * @return  sample size [bits]
 */
int AudioTest::getSampleSize() {
    std::string sufix = " bits";
    int samplesize = std::stoi(ui->sampleformat->currentText().toStdString().substr(0,ui->sampleformat->currentText().toStdString().find(sufix)));
    if (samplesize >= 0) {
        return samplesize;
    } else {
        consolelog("AudioTest",LogType::error,"invalid sample format has been selected");
        this->close();
    }
}

/**
 * @brief   It gets the selected sampling frequency
 * @return  sampling frequency [Hz]
 */
int AudioTest::getFS() {
    std::string sufix = " Hz";
    int fs = std::stoi(ui->fs->currentText().toStdString().substr(0,ui->fs->currentText().toStdString().find(sufix)));
    if (fs >= 0) {
        return fs;
    } else {
        consolelog("AudioTest",LogType::error,"invalid sample format has been selected");
        this->close();
    }
}

/**
 * @brief   It starts playing the test
 */

void AudioTest::start() {
    if (this->audiooutput) {
        delete this->audiooutput;
    }
    QObject::disconnect(ui->device,SIGNAL(currentIndexChanged(int)),this,SLOT(setDevice(int)));
    this->audiooutput = new AudioOutput(ui->device,this->getFS(),this->getSampleSize());
    QObject::connect(ui->device,SIGNAL(currentIndexChanged(int)),this,SLOT(setDevice(int)));
    this->play();
}

/**
 * @brief   It plays audio test.
 */
void AudioTest::play() {
    this->audiooutput->outputdevice->test((float)(ui->amplitude->value())/100,ui->frequency->value(),1);
    consolelog("AudioTest",LogType::progress,"playing test");
}
