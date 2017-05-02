// Class libraries
#include "Decoder.h"
#include "ui_Decoder.h"

int ChannelsList::fs;
int ChannelsList::samplesize;

/**
 * @brief	Decoder constructor.
 * @param   framework           Decoder user interface object
 */
Decoder::Decoder(QWidget *framework) :
    QMainWindow(framework),
    ui(new Ui::Decoder)
{
    // Initialization
    ui->setupUi(this);
    ChannelsList::fs = 44100;
    ChannelsList::samplesize = 32;
    int chunksize = 100;
    this->process = new ProcessManager(ChannelsList::fs, chunksize);
    // Effects
    this->effectsmonitor = new EffectsMonitor(ui->effect_monitor_list);
    std::map<std::string, Effect::effectID> effects = this->effectsmonitor->effects;
    for (std::map<std::string, Effect::effectID>::iterator iterator = effects.begin(); iterator != effects.end(); iterator++) {
        QAction *effect = new QAction(this);
        effect->setObjectName(ui->menu_effect->objectName() + QString::fromStdString("_" + iterator->first));
        effect->setText(QString::fromStdString(iterator->first));
        effect->setCheckable(true);
        ui->menu_effect->addAction(effect);
        QObject::connect(effect,SIGNAL(triggered(bool)),this,SLOT(toggleEffect()));
    }
    this->setEffect(effects.begin()->first);
    // Input & Output
    this->channels_input = new ChannelsList(ui->input_channels,0,false);
    this->channels_output = new ChannelsList(ui->output_channels,0,true);
    this->source = NULL;
    this->input = NULL;
    this->bitstream = NULL;
    this->setSource("");
    this->reset();
    float range[2][2] = {{0,(float)this->fs},{-1,1}};
    this->chart = new AudioChart(ui->input_chart,range,"",AudioChart::ChartOptions::logX);
    // Signals
    QObject::connect(ui->input_playback,SIGNAL(clicked(bool)),this,SLOT(setPlayback(bool)));
    QObject::connect(ui->output_playback,SIGNAL(clicked(bool)),this,SLOT(setPlayback(bool)));
    QObject::connect(ui->input_info,SIGNAL(released()),this,SLOT(openInfo()));
    // Menu - Source
    QObject::connect(ui->menu_encode,SIGNAL(triggered(bool)),this,SLOT(encode()));
    QObject::connect(ui->menu_load_source,SIGNAL(triggered(bool)),this,SLOT(load()));
    // Menu - Bitstream
    QObject::connect(ui->menu_load_bitstream,SIGNAL(triggered(bool)),this,SLOT(load()));
    // Menu - Decoder
    QObject::connect(ui->menu_bitstream_buried,SIGNAL(toggled(bool)),this,SLOT(setBuried(bool)));;
    QObject::connect(ui->menu_upmixtype,SIGNAL(triggered(QAction *)),this,SLOT(toggleUpmixType(QAction*)));
    QObject::connect(ui->menu_decodingtype,SIGNAL(triggered(QAction *)),this,SLOT(toggleDecodingType(QAction*)));
    QObject::connect(ui->menu_binauralquality,SIGNAL(triggered(QAction *)),this,SLOT(toggleBinauralQuality(QAction*)));
    QObject::connect(ui->menu_hrtfmodel,SIGNAL(triggered(QAction *)),this,SLOT(toggleHRTFModel(QAction*)));
    // Menu - Input
    QObject::connect(ui->menu_decode,SIGNAL(triggered(bool)),this,SLOT(decode()));
    // Menu - Effect

    // Menu - Output
    QObject::connect(ui->menu_test,SIGNAL(triggered(bool)),this,SLOT(test()));
    consolelog("Decoder", LogType::progress, "Decoder object is created");
}

/**
 * @brief	Decoder destructor.
 *
 */
Decoder::~Decoder()
{
    delete ui;
    consolelog("Decoder", LogType::progress, "Decoder object is deleted");
}

/**
 * @brief   It starts playing input.
 */
void Decoder::play() {

}

/**
 * @brief   It pauses input playback.
 */
void Decoder::pause() {

}

/**
 * @brief   It mutes output playback.
 */
void Decoder::mute() {
}

/**
 * @brief   It unmutes output playback.
 */
void Decoder::unmute() {

}

/**
 * @brief   It resets all decoding parameters, including input file.
 */
void Decoder::reset() {
    this->bitstream = NULL;
    this->input = NULL;
    this->setBitstream("");
    this->setInput("");
    this->setBuried(false);
    this->setUpmixType(UpmixType::normal);
    this->setDecodingType(DecodingType::low);
    this->setBinauralQuality(BinauralQuality::parametric);
    this->setHRTFModel(HRTFModel::kemar);
    consolelog("Decoder",LogType::interaction,"all decoding parameters has been reset as default");
}

/**
 * @brief   It updates enability of user interface controls according to the current parameters state.
 */
void Decoder::updateControls() {
    // Input controls
    ui->input_playback->setEnabled(this->input->exists());
    ui->input_info->setEnabled(this->input->exists());
    // Bitstream
    ui->menu_load_bitstream->setEnabled(this->source->exists() && !this->buried);
    // Decoder
    ui->menu_decode->setEnabled(this->source->exists() && this->bitstream->exists());
}

/**
 * @brief   It sets an effect for the effect monitor.
 * @param   effect              selected effect
 */
void Decoder::setEffect(std::string effect) {
    this->effectsmonitor->setEffect(effect);
    std::map<std::string, Effect::effectID> effects = this->effectsmonitor->effects;
    for (std::map<std::string, Effect::effectID>::iterator iterator = effects.begin(); iterator != effects.end(); iterator++) {
        QAction* action = this->findChild<QAction *>(ui->menu_effect->objectName() + QString::fromStdString("_" + iterator->first));
        action->setChecked(iterator->first == effect);
    }
}

/**
 * @brief   It sets the source audio file.
 * @param   filename            file path
 */
void Decoder::setSource(std::string filename) {
    if(filename != "") {
        this->source = new WAVFile(filename,false);
        this->reset();
    }
    bool loaded = this->source->exists();
    if(loaded && filename != "") {
        ui->source_filename->setText(QString::fromStdString(filename));
        consolelog("Decoder",LogType::progress,"selected \"" + filename + "\" as source file");
    } else if(loaded && filename == "") {
        ui->source_filename->setText(QString::fromStdString(this->source->getFilename()));
        consolelog("Decoder",LogType::interaction,"source file selection was cancelled");
    } else {
        ui->source_filename->setText(QString::fromStdString("please load a file"));
        consolelog("Decoder",LogType::warning,"source audio file is empty");
    }
    // File name font
    QFont font;
    font.setItalic(!loaded);
    ui->source_filename->setFont(font);
    // Controls update
    this->updateControls();
}

/**
 * @brief   It sets the bitstream audio file.
 * @param   filename            file path
 */
void Decoder::setBitstream(std::string filename) {
    if(filename != "") {
        this->bitstream = new File(filename, false);
    }
    bool loaded = this->bitstream;
    if (this->buried) {
        ui->bitstream_filename->setText("buried");
    } else {
        if(loaded && filename != "") {
            ui->bitstream_filename->setText(QString::fromStdString(filename));
            consolelog("Decoder",LogType::progress,"selected \"" + filename + "\" as bitstream file");
        } else if(loaded && filename == "") {
            ui->bitstream_filename->setText(QString::fromStdString(this->bitstream->getFilename()));
        } else {
            ui->bitstream_filename->setText(QString::fromStdString("please load a file"));
            consolelog("Decoder",LogType::warning,"bitstream file is empty");
        }
    }
    // File name font
    QFont font;
    font.setItalic(!loaded || this->buried);
    ui->bitstream_filename->setFont(font);
    // Controls update
    this->updateControls();
}

/**
 * @brief   It sets the input audio file.
 * @param   filename            file path
 */
void Decoder::setInput(std::string filename) {
    if(filename != "") {
        this->input = new WAVFile(filename, false);
        ChannelsList::fs = this->input->header.samplerate;
        ChannelsList::samplesize = this->input->header.bitspersample;
        this->channels_input->setChannelsNumber(this->input->header.numchannels);
        this->channels_output->setChannelsNumber(this->input->header.numchannels);
    }
    bool loaded = this->input->exists();
    if(loaded && filename != "") {
        ui->input_filename->setText(QString::fromStdString(filename));
        consolelog("Decoder",LogType::progress,"selected \"" + filename + "\" as input file");
    } else if(loaded && filename == "") {
        ui->input_filename->setText(QString::fromStdString(this->input->getFilename()));
        consolelog("Decoder",LogType::interaction,"input file selection was cancelled");
    } else {
        this->channels_input->setChannelsNumber(0);
        ui->input_filename->setText(QString::fromStdString("please decode source"));
        consolelog("Decoder",LogType::warning,"input file is empty");
    }
    // File name font
    QFont font;
    font.setItalic(!loaded);
    ui->input_filename->setFont(font);
    // Controls update
    this->updateControls();
}

/**
 * @brief   It sets SAC parameter upmix type
 * @param   upmixtype           upmix type
 */
void Decoder::setUpmixType(UpmixType::upmixtype upmixtype) {
    this->upmixtype = upmixtype;
    QList<QAction *> actions = ui->menu_upmixtype->actions();
    for (int item = 0; item < actions.length(); item++) {
        actions[item]->setChecked(false);
    }
    switch (upmixtype) {
    case UpmixType::normal:
    default:
        ui->menu_upmixtype_normal->setChecked(true);
        break;
    case UpmixType::blind:
        ui->menu_upmixtype_blind->setChecked(true);
        break;
    case UpmixType::binaural:
        ui->menu_upmixtype_binaural->setChecked(true);
        break;
    case UpmixType::stereo:
        ui->menu_upmixtype_stereo->setChecked(true);
        break;
    }
}

/**
 * @brief   It sets SAC parameter decoding type
 * @param   decodingtype        decoding type
 */
void Decoder::setDecodingType(DecodingType::decodingtype decodingtype) {
    this->decodingtype = decodingtype;
    QList<QAction *> actions = ui->menu_decodingtype->actions();
    for (int item = 0; item < actions.length(); item++) {
        actions[item]->setChecked(false);
    }
    switch (decodingtype) {
    case DecodingType::low:
    default:
        ui->menu_decodingtype_low->setChecked(true);
        break;
    case DecodingType::high:
        ui->menu_decodingtype_high->setChecked(true);
        break;
    }
}

/**
 * @brief   It sets SAC parameter binaural quality
 * @param   binauralquality     binaural quality
 */
void Decoder::setBinauralQuality(BinauralQuality::binauralquality binauralquality) {
    this->binauralquality = binauralquality;
    QList<QAction *> actions = ui->menu_binauralquality->actions();
    for (int item = 0; item < actions.length(); item++) {
        actions[item]->setChecked(false);
    }
    switch (binauralquality) {
    case BinauralQuality::parametric:
    default:
        ui->menu_binauralquality_parametric->setChecked(true);
        break;
    case BinauralQuality::filtering:
        ui->menu_binauralquality_filtering->setChecked(true);
        break;
    }
}

/**
 * @brief   It sets SAC parameter HRTF model
 * @param   hrtfmodel           HRTF model
 */
void Decoder::setHRTFModel(HRTFModel::hrtfmodel hrtfmodel) {
    this->hrtfmodel = hrtfmodel;
    QList<QAction *> actions = ui->menu_hrtfmodel->actions();
    for (int item = 0; item < actions.length(); item++) {
        actions[item]->setChecked(false);
    }
    switch (hrtfmodel) {
    case HRTFModel::kemar:
    default:
        ui->menu_hrtfmodel_kemar->setChecked(true);
        break;
    case HRTFModel::vast:
        ui->menu_hrtfmodel_vast->setChecked(true);
        break;
    case HRTFModel::mps_vt:
        ui->menu_hrtfmodel_mpsvt->setChecked(true);
        break;
    }
}

/**
 * @name    Audio menu bar interface slots
 * @brief   User interface functions for audio control.
 * @{
 */

/**
 * @brief   It loads a file as decoder input.
 */
void Decoder::load() {
    std::string prefix = "menu_load_";
    std::string file = QObject::sender()->objectName().toStdString().substr(QObject::sender()->objectName().toStdString().find(prefix) + prefix.length());
    std::string format = "";
    if(file=="source" || file =="input") {
        format = "*.wav";
    }
    std::string filename = QFileDialog::getOpenFileName(NULL, QString::fromStdString("Load " + file + " file"),"",QString::fromStdString(format)).toStdString();
    if(file=="source") {
        this->setSource(filename);
    } else if(file=="bitstream") {
        this->setBitstream(filename);
    }
}

/**
 * @brief   It encodes a new audio through Coder.
 */
void Decoder::encode() {
    Encoder *encoder = new Encoder();
    encoder->fs = this->fs;
    encoder->exec();
    if (encoder->output) {
        this->setSource(encoder->output->getFilename());
        if(encoder->bitstream->exists()) {
            this->setBitstream(encoder->bitstream->getFilename());
        }
        this->setBuried(!encoder->bitstream->exists());
    } else {
        this->setSource("");
    }
}

/**
 * @brief   It decodes the audio input.
 */
void Decoder::decode() {
    consolelog("Decoder",LogType::interaction,"decoding");
    std::string source = this->source->getFilename();
    std::string input = QFileDialog::getSaveFileName(NULL, "Save decoded input file","","*.wav").toStdString();
    std::string bitstream;
    if (this->buried) {
        bitstream = "buried";
    } else {
        bitstream = this->bitstream->getFilename();
    }
    if(input != "") {
        // SAC decoding
        consolelog("Decoder",LogType::progress,"proceeding to SAC decoder");
        bool successful = this->process->decode(source,bitstream, input, this->upmixtype, this->decodingtype, this->binauralquality, this->hrtfmodel);
        if (successful) {
            this->setInput(input);
        }
    }
}

/**
 * @brief   It controls the input and output playback.
 * @param   state               true to play and false to pause
 */
void Decoder::setPlayback(bool state) {
    QObject::sender()->blockSignals(true);
    std::string module = QObject::sender()->objectName().toStdString().substr(0,QObject::sender()->objectName().toStdString().find("_"));
    if (module == "input") {
        if (state) {
            consolelog("Decoder",LogType::interaction,"input is now playing");
        } else {
            consolelog("Decoder",LogType::interaction,"input is now paused");
        }
    } else if (module == "output") {
        if (state) {
            consolelog("Decoder",LogType::interaction,"unmuting output");
//            this->audiooutput->unmute();
        } else {
            consolelog("Decoder",LogType::interaction,"muting output");
//            this->audiooutput->mute();
        }
    }
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   It opens audio file info dialog.
 */
void Decoder::openInfo() {
    QObject::sender()->blockSignals(true);
    AudioInfo info;
    info.setFile(this->input);
    info.setWindowTitle("Input file info");
    info.exec();
    consolelog("Decoder",LogType::interaction,"showing input info");
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   It plays a test to a selected output audio device.
 */
void Decoder::test() {
    QObject::sender()->blockSignals(true);
    AudioTest audiotest;
    audiotest.exec();
    consolelog("Decoder",LogType::interaction,"showing audio output device test");
    QObject::sender()->blockSignals(false);
}

/**< @} */

/**
 * @name    Decoder menu bar interface slots
 * @brief   User interface control functions for SAC decoder parameters.
 * @{
 */

/**
 * @brief   It sets bitstream as buried or not.
 * @param   state               true if bitstream is buried
 */
void Decoder::setBuried(bool state) {
    this->buried = state;
    ui->menu_load_bitstream->setEnabled(!state);
    this->setBitstream("");
    if(state) {
        consolelog("Decoder",LogType::interaction,"bitstream has been set to buried");
    } else {
        consolelog("Decoder",LogType::interaction,"bitstream is not buried now");
    }
}

/**
 * @brief   Slot action for menu upmix type items
 * @param   item                selected item
 */
void Decoder::toggleUpmixType(QAction *item) {
    QObject::sender()->blockSignals(true);
    std::string prefix = "menu_upmixtype_";
    std::string type = item->objectName().toStdString().substr(prefix.length());
    if(type=="normal") {
        this->setUpmixType(UpmixType::normal);
    } else if(type=="blind") {
        this->setUpmixType(UpmixType::blind);
    } else if (type=="binaural") {
        this->setUpmixType(UpmixType::binaural);
    } else if (type=="stereo") {
        this->setUpmixType(UpmixType::stereo);
    } else {
        this->setUpmixType(UpmixType::normal);
    }
    consolelog("Decoder",LogType::interaction,"upmix type set to " + type);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slot action for menu decoding type items
 * @param   item                selected item
 */
void Decoder::toggleDecodingType(QAction *item) {
    QObject::sender()->blockSignals(true);
    std::string prefix = "menu_decodingtype_";
    std::string type = item->objectName().toStdString().substr(prefix.length());
    if(type=="low") {
        this->setDecodingType(DecodingType::low);
    } else if (type=="high") {
        this->setDecodingType(DecodingType::high);
    } else {
        this->setDecodingType(DecodingType::low);
    }
    consolelog("Decoder",LogType::interaction,"decoding type set to " + type);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slot action for menu binaural quality items
 * @param   item                selected item
 */
void Decoder::toggleBinauralQuality(QAction *item) {
    QObject::sender()->blockSignals(true);
    std::string prefix = "menu_binauralquality_";
    std::string type = item->objectName().toStdString().substr(prefix.length());
    if (type=="normal") {
        this->setBinauralQuality(BinauralQuality::parametric);
    } else if (type=="blind") {
        this->setBinauralQuality(BinauralQuality::filtering);
    } else {
        this->setBinauralQuality(BinauralQuality::parametric);
    }
    consolelog("Decoder",LogType::interaction,"binaural quality set to " + type);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slot action for menu HRTF model items
 * @param   item                selected item
 */
void Decoder::toggleHRTFModel(QAction *item) {
    QObject::sender()->blockSignals(true);
    std::string prefix = "menu_hrtfmodel_";
    std::string type = item->objectName().toStdString().substr(prefix.length());
    if (type=="kemar") {
        this->setHRTFModel(HRTFModel::kemar);
    } else if(type=="vast") {
        this->setHRTFModel(HRTFModel::vast);
    } else if(type=="mpsvt") {
        this->setHRTFModel(HRTFModel::mps_vt);
    } else {
        this->setHRTFModel(HRTFModel::kemar);
    }
    consolelog("Decoder",LogType::interaction,"HRTF model set to " + type);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slot action for menu effect items
 */
void Decoder::toggleEffect() {
    QObject::sender()->blockSignals(true);
    std::string effect = QObject::sender()->objectName().toStdString().substr(QObject::sender()->objectName().toStdString().find(ui->menu_effect->objectName().toStdString() + "_") + ui->menu_effect->objectName().toStdString().length() + 1);
    std::map<std::string, Effect::effectID> effects = this->effectsmonitor->effects;
    for (std::map<std::string, Effect::effectID>::iterator iterator = effects.begin(); iterator != effects.end(); iterator++) {
        if (iterator->first == effect) {
            this->setEffect(iterator->first);
            consolelog("Decoder",LogType::interaction, "selecting effect \"" + iterator->first + "\"");
            QObject::sender()->blockSignals(false);
            return;
        }
    }
    consolelog("Decoder",LogType::error,"effect \"" + effect + "\" is not available");
    QObject::sender()->blockSignals(false);
}

/** @} */
