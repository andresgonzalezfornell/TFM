/**
 * @name	input.cpp
 * @author	Andrés González Fornell
 * @brief	System input audio devices class.
 */

// Class libraries
#include "input.h"
#include "ui_mainwindow.h"
// System libraries
#include "math.h"
#include "stdlib.h"
// Qt libraries
#include "QDateTime"
#include "QDebug"
#include "QPainter"
#include "QVBoxLayout"
#include "QAudioDeviceInfo"
#include "QAudioInput"
#include "qendian.h"

const int BufferSize = 4096;

/**
 * @brief	AudioInput constructor.
 */
InputChannel::InputChannel(const QAudioFormat &format, QObject *parent)
    :   QIODevice(parent)
    ,   audioformat(format)
    ,   amplitude(0)
    ,   level(0.0)
{
    switch (audioformat.sampleSize()) {
    case 8:
        switch (audioformat.sampleType()) {
        case QAudioFormat::UnSignedInt:
            amplitude = 255;
            break;
        case QAudioFormat::SignedInt:
            amplitude = 127;
            break;
        default:
            break;
        }
        break;
    case 16:
        switch (audioformat.sampleType()) {
        case QAudioFormat::UnSignedInt:
            amplitude = 65535;
            break;
        case QAudioFormat::SignedInt:
            amplitude = 32767;
            break;
        default:
            break;
        }
        break;
    case 32:
        switch (audioformat.sampleType()) {
        case QAudioFormat::UnSignedInt:
            amplitude = 0xffffffff;
            break;
        case QAudioFormat::SignedInt:
            amplitude = 0x7fffffff;
            break;
        case QAudioFormat::Float:
            amplitude = 0x7fffffff;
        default:
            break;
        }
        break;

    default:
        break;
    }
    consolelog("input",progress,"AudioInput object is created");
}

/**
 * @brief	AudioInput destructor.
 */
InputChannel::~InputChannel()
{
    consolelog("input",progress,"AudioInput object is deleted");
}

/**
 * @brief	It starts input channel.
 */
void InputChannel::start()
{
    open(QIODevice::WriteOnly);
    consolelog("input",progress,"AudioInput has started");
}

/**
 * @brief	It stops input channel.
 */
void InputChannel::stop()
{
    close();
    consolelog("input",progress,"AudioInput has stopped");
}

/**
 * @brief	It reads data from the input channel.
 * @param   *data       data pointer
 * @param   maxlen      maximum lenght
 * @return
 */
qint64 InputChannel::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data)
    Q_UNUSED(maxlen)
    return 0;
}

/**
 * @brief	It writes data from the input channel.
 * @param   *data       data pointer
 * @param   len         data length
 * @return
 */
qint64 InputChannel::writeData(const char *data, qint64 len)
{
    if (amplitude) {
        Q_ASSERT(audioformat.sampleSize() % 8 == 0);
        const int channelBytes = audioformat.sampleSize() / 8;
        const int sampleBytes = audioformat.channelCount() * channelBytes;
        Q_ASSERT(len % sampleBytes == 0);
        const int numSamples = len / sampleBytes;

        quint32 value_max = 0;
        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

        for (int i = 0; i < numSamples; ++i) {
            for (int j = 0; j < audioformat.channelCount(); ++j) {
                quint32 value = 0;
                if (audioformat.sampleSize() == 8 && audioformat.sampleType() == QAudioFormat::UnSignedInt) {
                    value = *reinterpret_cast<const quint8*>(ptr);
                } else if (audioformat.sampleSize() == 8 && audioformat.sampleType() == QAudioFormat::SignedInt) {
                    value = qAbs(*reinterpret_cast<const qint8*>(ptr));
                } else if (audioformat.sampleSize() == 16 && audioformat.sampleType() == QAudioFormat::UnSignedInt) {
                    if (audioformat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint16>(ptr);
                    else
                        value = qFromBigEndian<quint16>(ptr);
                } else if (audioformat.sampleSize() == 16 && audioformat.sampleType() == QAudioFormat::SignedInt) {
                    if (audioformat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint16>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint16>(ptr));
                } else if (audioformat.sampleSize() == 32 && audioformat.sampleType() == QAudioFormat::UnSignedInt) {
                    if (audioformat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint32>(ptr);
                    else
                        value = qFromBigEndian<quint32>(ptr);
                } else if (audioformat.sampleSize() == 32 && audioformat.sampleType() == QAudioFormat::SignedInt) {
                    if (audioformat.byteOrder() == QAudioFormat::LittleEndian)
                        value = qAbs(qFromLittleEndian<qint32>(ptr));
                    else
                        value = qAbs(qFromBigEndian<qint32>(ptr));
                } else if (audioformat.sampleSize() == 32 && audioformat.sampleType() == QAudioFormat::Float) {
                    value = qAbs(*reinterpret_cast<const float*>(ptr) * 0x7fffffff); // assumes 0-1.0
                }

                value_max = qMax(value, value_max);
                ptr += channelBytes;
            }
        }
        value_max = qMin(value_max, amplitude);
        level = qreal(value_max) / amplitude;
    }
    emit update();
    return len;
}

/**
 * @brief	It gets the current signal level.
 * @return  level
 */
qreal InputChannel::getLevel()
{
    return level;
}

/**
 * @brief	InputLevel constructor.
 * @param   *element    user interface element pointer to place the volumeter
 */
InputLevel::InputLevel(QWidget *parent)
    : QWidget(parent)
{
    width = parent->frameGeometry().width();
    level = 0;
    consolelog("input",progress,"InputLevel object is created");
}

/**
 * @brief	It fills the volumeter.
 */
void InputLevel::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    // Size
    int x = painter.viewport().left();
    int y = painter.viewport().top();
    int width = this->width;
    int height = painter.viewport().bottom();
    // Fill color
    QLinearGradient gradient = QLinearGradient(0,0,width,0);
    gradient.setColorAt(0, QColor::fromRgbF(0.2941, 0.6706, 0.1882, 1));    // Low level color
    gradient.setColorAt(1, QColor::fromRgbF(0.8314, 0.1373, 0.0863, 1));    // High level color
    QBrush color_fill = QBrush(gradient);
    // Contour color
    QColor color_contour = QColor::fromRgbF(0,0,0,0);
    painter.setPen(QPen(color_contour));
    painter.drawRect(QRect(x,y,width,height));
    if (level != 0) {
        painter.fillRect(x,y,width*level,height,color_fill);
    }
}

/**
 * @brief	It sets the new level value to the volumeter
 * @param   value
 */
void InputLevel::setLevel(qreal value)
{
    level = value;
    update();
}

/**
 * @brief	Input constructor.
 * @param   &framework  user interface framework of input
 */
Input::Input(QWidget *framework) :
    device(QAudioDeviceInfo::defaultInputDevice()),
    channel(0),
    audioinput(0)
{
    // Data channel
    initialize();
    // Ui
    QComboBox *input_device = framework->findChild<QComboBox *>("input_device");
    updateDevices(*input_device);
    volumeter = new InputLevel(framework->findChild<QWidget *>("input_level"));
    playPause(framework->findChild<QPushButton *>("input_pause"));
    consolelog("input",progress,"Input object is created");
}

/**
 * @brief	Input destructor.
 */
Input::~Input() {
    channel->stop();
    audioinput->stop();
    consolelog("input",progress,"Input object is deleted");
}

/**
 * @brief	It initializes audio input.
 */
void Input::initialize()
{
    consolelog("Input",progress,"initializing input");
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");
    QAudioDeviceInfo info(device);
    if (!info.isFormatSupported(format)) {
        format = info.nearestFormat(format);
        consolelog("Input",warning,"default format not supported - trying to use nearest");
    }
    if (channel) {
        delete channel;
    }
    channel = new InputChannel(format, this);
    connect(channel, SIGNAL(update()),SLOT(refresh()));
    audioinput = new QAudioInput(device, format, this);
    channel->start();
    audioinput->start(channel);
}

/**
 * @brief	It is called when input data has been received.
 */
void Input::refresh() {
    volumeter->setLevel(channel->getLevel());
}

/**
 * @brief	It updates system input devices combobox.
 * @param   input_device    QComboBox object to update
 */
void Input::updateDevices(QComboBox &input_device) {
    input_device.blockSignals(true);
    for (int index = 0;index < input_device.count();index++) {
        input_device.removeItem(index);
    }
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    for(int index = 0;index < devices.count();index++) {
        input_device.addItem(devices.value(index).deviceName());
        consolelog("input",info,"Input device updated: " + devices.value(index).deviceName().toStdString());
    }
    input_device.blockSignals(false);
}

/**
 * @brief	It gets selected input audio device.
 * @param   item            device index
 * @return  QAudioDeviceInfo selected device.
 */
QAudioDeviceInfo Input::getDevice(int index) {
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    return devices.value(index);
}

/**
 * @brief	It changes input audio device.
 * @param   item            Selected device index
 */
void Input::setDevice(int index) {
    channel->stop();
    audioinput->stop();
    audioinput->disconnect(this);
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    QAudioDeviceInfo device = devices.value(index);
    consolelog("input",progress,"input device changed to " + device.deviceName().toStdString());
    initialize();
}

/**
 * @brief	Change input audio device.
 * @param   item            Selected device index
 */
void Input::setVolume(int value)
{
        qreal volume = value/100;
        audioinput->setVolume(volume);
}

/**
 * @brief	Input play/pause action.
 * @param   button
 */
void Input::playPause(QPushButton *button) {
    QString text_old = button->text();
    QString text_new = "";
    if (text_old.toStdString()=="Play") {
        text_new = "Pause";
        audioinput->resume();
    } else if(text_old.toStdString()=="Pause") {
        text_new = "Play";
        audioinput->suspend();
    } else {
        text_new = "Pause";
        audioinput->resume();
    }
    button->setText(text_new);
    consolelog("input",info,"input play/pause button is showing now: " + text_new.toStdString());
}

/**
 * @brief	Input push/pull mode action.
 * @param   button
 */
void Input::switchMode(QPushButton *button) {
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
    consolelog("input",info,"input play/pause button is showing now: " + text_new.toStdString());
}
