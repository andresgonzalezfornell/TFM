// Class libraries
#include "SACEffects.h"
#include "ui_SACEffects.h"

int ChannelsList::fs;
int ChannelsList::samplesize;

/**
 * @brief	SACEffects constructor.
 * @param   framework           SACEffects user interface object
 */
SACEffects::SACEffects(QWidget *framework) :
    QMainWindow(framework), ui(new Ui::SACEffects) {
    // Initialization
    ui->setupUi(this);
    ChannelsList::fs = this->fs;
    ChannelsList::samplesize = 32;
    this->chunksize = 4410;
    consolelog("SACEffects", LogType::info,
               "signal process chunk = " + std::to_string(this->chunksize)
               + " samples");
    this->process = new ProcessManager(this->fs, chunksize);
    this->chart_input = new Chart2D(ui->input_chart);
    this->chart_output = new Chart2D(ui->output_chart);
    // Clock
    this->clock = new QTimer(this);
    double period = (double) chunksize / this->fs;
    consolelog("SACEffects", LogType::info,
               "realtime cycle has a period = "
               + std::to_string(period).substr(0,
                                               std::to_string(period).find(".") + 4) + " s");
    this->clock->setInterval(period * 1000);
    QObject::connect(this->clock, SIGNAL(timeout()), this, SLOT(applyEffect()));
    QObject::connect(this->clock, SIGNAL(timeout()), this, SLOT(sendOutput()));
    QObject::connect(this->clock, SIGNAL(timeout()), this, SLOT(setTimer()));
    // Effects
    this->effectsmonitor = new EffectsMonitor(ui->effect_monitor_list);
    std::map<Effect::effectID, std::string> effects =
            this->effectsmonitor->effects;
    // Input & Output
    this->channels_input = new ChannelsList(ui->input_channels, 0, false);
    this->channels_output = new ChannelsList(ui->output_channels, 0, true);
    this->input = NULL;
    this->bitstream = NULL;
    this->setSource("");
    this->reset();
    // Signals - Source
    QObject::connect(ui->menu_encode,SIGNAL(triggered(bool)),this,SLOT(encode()));
    QObject::connect(ui->menu_load_source,SIGNAL(triggered(bool)),this,SLOT(load()));
    // Signals - Bitstream
    QObject::connect(ui->menu_load_bitstream,SIGNAL(triggered(bool)),this,SLOT(load()));
    // Signals - SACEffects
    QObject::connect(ui->menu_bitstream_buried,SIGNAL(toggled(bool)),this,SLOT(setBuried(bool)));;
    QObject::connect(ui->menu_upmixtype,SIGNAL(triggered(QAction *)),this,SLOT(toggleUpmixType(QAction*)));
    QObject::connect(ui->menu_decodingtype,SIGNAL(triggered(QAction *)),this,SLOT(toggleDecodingType(QAction*)));
    QObject::connect(ui->menu_binauralquality,SIGNAL(triggered(QAction *)),this,SLOT(toggleBinauralQuality(QAction*)));
    QObject::connect(ui->menu_hrtfmodel,SIGNAL(triggered(QAction *)),this,SLOT(toggleHRTFModel(QAction*)));
    // Signals - Input
    QObject::connect(this->channels_input, SIGNAL(namechanged()), this,
                     SLOT(updateInputChannels()));
    QObject::connect(ui->input_playback,SIGNAL(clicked(bool)),this,SLOT(setPlayback(bool)));
    QObject::connect(ui->input_stop,SIGNAL(clicked(bool)),this,SLOT(stop()));
    QObject::connect(ui->input_info, SIGNAL(released()), this,
                     SLOT(openInfo()));
    QObject::connect(ui->input_timer, SIGNAL(timeChanged(QTime)), this,
                     SLOT(setTimer(QTime)));
    QObject::connect(ui->output_timer, SIGNAL(timeChanged(QTime)), this,
                     SLOT(setTimer(QTime)));
    QObject::connect(ui->menu_decode,SIGNAL(triggered(bool)),this,SLOT(decode()));
    // Signals - Effect
    for (std::map<Effect::effectID, std::string>::iterator iterator =
         effects.begin(); iterator != effects.end(); iterator++) {
        QAction *effect = new QAction(this);
        effect->setObjectName(
                    ui->menu_effect->objectName()
                    + QString::fromStdString("_" + iterator->second));
        effect->setText(QString::fromStdString(iterator->second));
        effect->setCheckable(true);
        ui->menu_effect->addAction(effect);
        QObject::connect(effect,SIGNAL(triggered(bool)),this,SLOT(toggleEffect()));
    }
    this->setEffect(effects.begin()->first);
    // Signals - Output
    QObject::connect(this->channels_output, SIGNAL(namechanged()), this,
                     SLOT(updateOutputChannels()));
    QObject::connect(ui->output_playback,SIGNAL(clicked(bool)),this,SLOT(setPlayback(bool)));
    QObject::connect(ui->menu_export,SIGNAL(triggered(bool)),this,SLOT(exportOutput()));
    QObject::connect(ui->menu_test,SIGNAL(triggered(bool)),this,SLOT(test()));
    consolelog("SACEffects", LogType::progress, "SACEffects object is created");
}

/**
 * @brief	SACEffects destructor.
 *
 */
SACEffects::~SACEffects() {
    delete ui;
    consolelog("main", LogType::progress, "closing SAC Effects");
}

/**
 * @brief   It starts playing input.
 */
void SACEffects::play() {
    this->clock->start();
    ui->input_playback->setChecked(true);
    ui->input_timer->setReadOnly(true);
}

/**
 * @brief   It pauses input playback.
 */
void SACEffects::pause() {
    this->clock->stop();
    ui->input_playback->setChecked(false);
    ui->input_timer->setReadOnly(false);
    this->plot();
}

/**
 * @brief   It stops input playback
 */
void SACEffects::stop() {
    this->pause();
    this->process->cursor = 0;
    this->setTimer();
    this->chart_input->clear();
    this->chart_output->clear();
}

/**
 * @brief   It resets all decoding parameters, including input file.
 */
void SACEffects::reset() {
    this->bitstream = NULL;
    this->input = NULL;
    this->setBitstream("");
    this->setInput("");
    this->setDuration(ui->output_duration, 0);
    this->setBuried(false);
    this->setUpmixType(UpmixType::normal);
    this->setDecodingType(DecodingType::low);
    this->setBinauralQuality(BinauralQuality::parametric);
    this->setHRTFModel(HRTFModel::kemar);
    consolelog("SACEffects", LogType::interaction,
               "all decoding parameters has been reset as default");
}

/**
 * @brief   It updates enability of user interface controls according to the current parameters state.
 */
void SACEffects::updateControls() {
    // SACEffects
    ui->menu_decode->setEnabled(
                this->source->exists() && this->bitstream->exists());
    // Bitstream
    ui->menu_load_bitstream->setEnabled(this->source->exists() && !this->buried);
    // Input & output controls
    ui->menu_export->setEnabled(this->input->exists());
    ui->input_playback->setEnabled(this->input->exists());
    ui->output_playback->setEnabled(this->input->exists());
    ui->input_stop->setEnabled(this->input->exists());
    ui->input_info->setEnabled(this->input->exists());
    ui->input_timer->setEnabled(this->input->exists());
    ui->output_timer->setEnabled(this->input->exists());
    if (this->input->exists()) {
        double time = 1000 * this->input->duration; // current playback time [ms]
        int h = (int) std::floor(time / 1000 / 60 / 60);
        int m = (int) std::floor(time / 1000 / 60) % 60;
        int s = (int) std::floor(time / 1000) % 60;
        int ms = (int) std::round(time) % 1000;
        ui->input_timer->setMaximumTime(QTime(h, m, s, ms));
        ui->output_timer->setMaximumTime(QTime(h, m, s, ms));
    } else {
        ui->input_timer->setTime(QTime(0, 0, 0, 0));
        ui->output_timer->setTime(QTime(0, 0, 0, 0));
    }
}

/**
 * @brief   It sets an effect for the effect monitor.
 * @param   effect              selected effect
 */
void SACEffects::setEffect(Effect::effectID effect) {
    delete this->effect;
    this->effect = new Effect(effect);
    this->effectsmonitor->setEffect(this->effect);
    std::map<Effect::effectID, std::string> effects = this->effectsmonitor->effects;
    for (std::map<Effect::effectID, std::string>::iterator iterator =
        effects.begin(); iterator != effects.end(); iterator++) {
        QAction* action = this->findChild<QAction *>(
                    ui->menu_effect->objectName()
                    + QString::fromStdString("_" + iterator->second));
        action->setChecked(iterator->first == effect);
    }
}

/**
 * @brief   It sets the source audio file.
 * @param   filename            file path
 */
void SACEffects::setSource(std::string filename) {
    if (filename != "") {
        this->source = new WAVFile(filename, false);
        this->reset();
    }
    bool loaded = this->source->exists();
    if (loaded && filename != "") {
        ui->source_filename->setText(QString::fromStdString(filename));
        consolelog("SACEffects", LogType::progress,
                   "selected \"" + filename + "\" as source file");
    } else if (loaded && filename == "") {
        ui->source_filename->setText(
                    QString::fromStdString(this->source->getFilename()));
        consolelog("SACEffects", LogType::interaction,
                   "source file selection was cancelled");
    } else {
        ui->source_filename->setText(QString::fromStdString("please load a file"));
        consolelog("SACEffects", LogType::warning, "source audio file is empty");
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
void SACEffects::setBitstream(std::string filename) {
    if (filename != "") {
        this->bitstream = new File(filename, false);
    }
    bool loaded = this->bitstream;
    if (this->buried) {
        ui->bitstream_filename->setText("buried");
    } else {
        if (loaded && filename != "") {
            ui->bitstream_filename->setText(QString::fromStdString(filename));
            consolelog("SACEffects", LogType::progress,
                       "selected \"" + filename + "\" as bitstream file");
        } else if (loaded && filename == "") {
            ui->bitstream_filename->setText(
                        QString::fromStdString(this->bitstream->getFilename()));
        } else {
            ui->bitstream_filename->setText(
                        QString::fromStdString("please load a file"));
            consolelog("SACEffects", LogType::warning, "bitstream file is empty");
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
void SACEffects::setInput(std::string filename) {
    if (filename != "") {
        this->input = new WAVFile(filename, false);
        ChannelsList::fs = this->input->header.samplerate;
        ChannelsList::samplesize = this->input->header.bitspersample;
        this->channels_input->setSize(this->input->header.numchannels);
        this->channels_output->setSize(this->input->header.numchannels);
    }
    bool loaded = this->input->exists();
    if (loaded && filename != "") {
        ui->input_filename->setText(QString::fromStdString(filename));
        this->setDuration(ui->input_duration, this->input->duration);
        consolelog("SACEffects", LogType::progress,
                   "selected \"" + filename + "\" as input file");
    } else if (loaded && filename == "") {
        ui->input_filename->setText(
                    QString::fromStdString(this->input->getFilename()));
        consolelog("SACEffects", LogType::interaction,
                   "input file selection was cancelled");
    } else {
        this->channels_input->setSize(0);
        ui->input_filename->setText(QString::fromStdString("please decode source"));
        this->setDuration(ui->input_duration, 0);
        consolelog("SACEffects", LogType::warning, "input file is empty");
    }
    // File name font
    QFont font;
    font.setItalic(!loaded);
    ui->input_filename->setFont(font);
    // Controls update
    this->updateInputChannels();
    this->updateOutputChannels();
    this->updateControls();
}

/**
 * @brief   It sets a duration indicator text on an user interface label object.
 * @param   label               user interface object where to indicate duration
 * @param   duration            input audio file duration [s]
 */
void SACEffects::setDuration(QLabel *label, double duration) {
    int h = (int) std::floor(duration / 60 / 60);
    int m = (int) std::floor(duration / 60) % 60;
    int s = (int) std::floor(duration) % 60;
    int ms = (int) std::round(duration * 1000) % 1000;
    std::string text = "/ ";
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
    label->setText(QString::fromStdString(text));
}

/**
 * @brief   It sets SAC parameter upmix type
 * @param   upmixtype           upmix type
 */
void SACEffects::setUpmixType(UpmixType::upmixtype upmixtype) {
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
void SACEffects::setDecodingType(DecodingType::decodingtype decodingtype) {
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
void SACEffects::setBinauralQuality(
        BinauralQuality::binauralquality binauralquality) {
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
void SACEffects::setHRTFModel(HRTFModel::hrtfmodel hrtfmodel) {
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
 * @brief   It sends the current output signal to the output device and plots on the output chart.
 */
void SACEffects::sendOutput() {
    int N; // number of samples to be plotted
    int n; // first sample to be plotted
    if (this->chunksize == 0) {
        N = this->process->samples;
        n = 0;
    } else {
        N = this->chunksize;
        n = this->process->cursor - N;
        if (n + N >= this->process->samples) {
            N = this->process->samples - n - 1;
        }
    }
    for (int channel = 0; channel < this->channels_output->getSize(); channel++) {
        if (!muted) {
            this->channels_output->getChannel(channel)->audiooutput->outputdevice->send(process->output[channel] + n, N);
        }
    }
    this->plot();
}

/**
 * @brief   It plots the current input and ouput signals.
 */
void SACEffects::plot() {
    Chart2D *chart;
    QVector < QPointF > points;
    float *samples;
    int n = 0;
    int N = 0;
    int remainder = 0;
    double range[2][2];
    for (int plot = 0; plot < 2; plot++) {
        switch (plot) {
        case 0:
            // Input
            samples = this->process->input[(int)std::floor(ui->input_chart_selector->currentIndex() / 2)];
            remainder = (int)ui->input_chart_selector->currentIndex() % 2;
            chart = this->chart_input;
            break;
        case 1:
            // Output
            samples = this->process->output[(int)std::floor(ui->output_chart_selector->currentIndex() / 2)];
            remainder = (int)ui->output_chart_selector->currentIndex() % 2;
            chart = this->chart_output;
            break;
        }
        switch (remainder) {
        case 0:
            // Signal plot
            range[0][0] = 0;
            range[0][1] = (double) (N - 1) / this->fs ;
            range[1][0] = -1;
            range[1][1] = 1;
            chart->setOptions(Chart2D::ChartOptions::labelX);
            for (int sample = 0; sample < N; sample++) {
                points.push_back(QPointF((float) sample / this->fs, samples[n + sample]));
            }
            break;
        case 1:
            // Spectrum plot
            std::vector<float> vector = std::vector<float>(samples[n], samples[n + N]);;
            AudioSignal signal = AudioSignal(vector, this->fs);
            range[0][0] = 0;
            range[0][1] = (double) this->fs / 2;
            range[1][0] = 0;
            range[1][1] = 1;
            chart->setOptions(Chart2D::ChartOptions::labelX);
            std::vector<float> spectrum = signal.getSpectrum();
            for (int frequency = 0; frequency < std::ceil((double) N / 2); frequency++) {
                points.push_back(QPointF((float) frequency * this->fs / N, spectrum[frequency]));
            }
            break;
        }
        chart->clear();
        chart->setRange(range);
        chart->setPoints(points);
    }
}

/**
 * @brief   It loads current input channels name on chart selector.
 */
void SACEffects::updateInputChannels() {
    ui->input_chart_selector->clear();
    std::vector<std::string> names = this->channels_input->getNames();
    for (int index = 0; index < this->channels_input->getSize(); index++) {
        ui->input_chart_selector->addItem(
                    QString::fromStdString(names[index] + " - Signal"));
        ui->input_chart_selector->addItem(
                    QString::fromStdString(names[index] + " - Spectrum"));
    }
}

/**
 * @brief   It loads current output channels name on chart selector.
 */
void SACEffects::updateOutputChannels() {
    ui->output_chart_selector->clear();
    std::vector<std::string> names = this->channels_output->getNames();
    for (int index = 0; index < this->channels_output->getSize(); index++) {
        ui->output_chart_selector->addItem(
                    QString::fromStdString(names[index]) + " - Signal");
        ui->output_chart_selector->addItem(
                    QString::fromStdString(names[index]) + " - Spectrum");
    }
}

/**
 * @name    Playback controls slots
 * @brief   User interface functions for audio playback control.
 * @{
 */

/**
 * @brief   It controls the input and output playback.
 * @param   state               true to play and false to pause
 */
void SACEffects::setPlayback(bool state) {
    QObject::sender()->blockSignals(true);
    std::string module = QObject::sender()->objectName().toStdString().substr(0,
                                                                              QObject::sender()->objectName().toStdString().find("_"));
    if (module == "input") {
        if (state) {
            this->play();
            consolelog("SACEffects", LogType::interaction, "playing input");
        } else {
            this->pause();
            consolelog("SACEffects", LogType::interaction, "paused input");
        }
    } else if (module == "output") {
        this->muted = !state;
        if (state) {
            consolelog("SACEffects", LogType::interaction, "unmuting output");
        } else {
            consolelog("SACEffects", LogType::interaction, "muting output");
        }
    }
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   It opens audio file info dialog.
 */
void SACEffects::openInfo() {
    QObject::sender()->blockSignals(true);
    AudioInfo info;
    info.setFile(this->input);
    info.setWindowTitle("Input file info");
    info.exec();
    consolelog("SACEffects", LogType::interaction, "showing input info dialog");
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   It sets audio playback time from the cursor.
 */
void SACEffects::setTimer() {
    if (this->process->cursor < this->process->samples) {
        double time = (double) 1000 * this->process->cursor / this->fs; // current playback time [ms]
        int h = (int) std::floor(time / 1000 / 60 / 60);
        int m = (int) std::floor(time / 1000 / 60) % 60;
        int s = (int) std::floor(time / 1000) % 60;
        int ms = (int) std::round(time) % 1000;
        ui->input_timer->blockSignals(true);
        ui->output_timer->blockSignals(true);
        ui->input_timer->setTime(QTime(h, m, s, ms));
        ui->output_timer->setTime(QTime(h, m, s, ms));
        ui->input_timer->blockSignals(false);
        ui->output_timer->blockSignals(false);
        if (this->process->cursor == this->process->total) {
            this->setDuration(ui->output_duration, (double) time / 1000);
        }
    } else {
        this->stop();
    }
}

/**
 * @brief   It sets audio playback time from a specific time selected by user.
 * @param   time
 */
void SACEffects::setTimer(QTime time) {
    QObject::sender()->blockSignals(true);
    long int ms = 0;
    ms += 1000 * 60 * 60 * time.hour();
    ms += 1000 * 60 * time.minute();
    ms += 1000 * time.second();
    ms += time.msec();
    this->process->cursor = std::floor((double) ms / 1000 * this->fs);
    ui->input_timer->setTime((time));
    ui->output_timer->setTime((time));
    QObject::sender()->blockSignals(false);
    consolelog("SACEffects", LogType::interaction,
               "audio playback time set to " + std::to_string(time.hour()) + "h "
               + std::to_string(time.minute()) + "m "
               + std::to_string(time.second()) + "."
               + std::to_string(time.msec()) + "s");
}

/** @} */

/**
 * @name    Audio menu bar interface slots
 * @brief   User interface functions for audio control.
 * @{
 */

/**
 * @brief   It loads a file as decoder input.
 */
void SACEffects::load() {
    std::string prefix = "menu_load_";
    std::string file = QObject::sender()->objectName().toStdString().substr(
                QObject::sender()->objectName().toStdString().find(prefix)
                + prefix.length());
    std::string format = "";
    if (file == "source" || file == "input") {
        format = "*.wav";
    }
    std::string filename = QFileDialog::getOpenFileName(NULL,
                                                        QString::fromStdString("Load " + file + " file"), "",
                                                        QString::fromStdString(format)).toStdString();
    if (file == "source") {
        this->setSource(filename);
    } else if (file == "bitstream") {
        this->setBitstream(filename);
    }
}

/**
 * @brief   It exports generated output singal as audio file.
 */
void SACEffects::exportOutput() {
    std::string filename =
            QFileDialog::getSaveFileName(NULL,
                                         QString::fromStdString("Export output as audio file"), "",
                                         "*.wav").toStdString();
    if (filename != "") {
        this->process->setOutput(filename);
        consolelog("SACEffects", LogType::interaction,
                   "output was exported to \"" + filename + "\"");
    } else {
        consolelog("SACEffects", LogType::interaction,
                   "output export was cancelled");
    }
}

/**
 * @brief   It plays a test to a selected output audio device.
 */
void SACEffects::test() {
    QObject::sender()->blockSignals(true);
    AudioTest audiotest;
    audiotest.exec();
    consolelog("SACEffects", LogType::interaction,
               "showing audio output device test");
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   It encodes a new audio through Coder.
 */
void SACEffects::encode() {
    Encoder *encoder = new Encoder();
    encoder->fs = this->fs;
    encoder->exec();
    if (encoder->output) {
        this->setSource(encoder->output->getFilename());
        if (encoder->bitstream->exists()) {
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
void SACEffects::decode() {
    consolelog("SACEffects", LogType::interaction, "decoding");
    std::string source = this->source->getFilename();
    std::string input = QFileDialog::getSaveFileName(NULL,
                                                     "Save decoded input file", "", "*.wav").toStdString();
    std::string bitstream;
    if (this->buried) {
        bitstream = "buried";
    } else {
        bitstream = this->bitstream->getFilename();
    }
    if (input != "") {
        // SAC decoding
        consolelog("SACEffects", LogType::progress, "proceeding to SAC decoder");
        bool successful = this->process->decode(source, bitstream, input,
                                                this->upmixtype, this->decodingtype, this->binauralquality,
                                                this->hrtfmodel);
        if (successful) {
            this->setInput(input);
        }
    }
}

/**< @} */

/**
 * @name    SACEffects menu bar interface slots
 * @brief   User interface control functions for SAC decoder parameters.
 * @{
 */

/**
 * @brief   It sets bitstream as buried or not.
 * @param   state               true if bitstream is buried
 */
void SACEffects::setBuried(bool state) {
    this->buried = state;
    ui->menu_load_bitstream->setEnabled(!state);
    this->setBitstream("");
    if (state) {
        consolelog("SACEffects", LogType::interaction,
                   "bitstream has been set to buried");
    } else {
        consolelog("SACEffects", LogType::interaction,
                   "bitstream is not buried now");
    }
}

/**
 * @brief   Slot action for menu upmix type items
 * @param   item                selected item
 */
void SACEffects::toggleUpmixType(QAction *item) {
    QObject::sender()->blockSignals(true);
    std::string prefix = "menu_upmixtype_";
    std::string type = item->objectName().toStdString().substr(prefix.length());
    if (type == "normal") {
        this->setUpmixType(UpmixType::normal);
    } else if (type == "blind") {
        this->setUpmixType(UpmixType::blind);
    } else if (type == "binaural") {
        this->setUpmixType(UpmixType::binaural);
    } else if (type == "stereo") {
        this->setUpmixType(UpmixType::stereo);
    } else {
        this->setUpmixType(UpmixType::normal);
    }
    consolelog("SACEffects", LogType::interaction, "upmix type set to " + type);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slot action for menu decoding type items
 * @param   item                selected item
 */
void SACEffects::toggleDecodingType(QAction *item) {
    QObject::sender()->blockSignals(true);
    std::string prefix = "menu_decodingtype_";
    std::string type = item->objectName().toStdString().substr(prefix.length());
    if (type == "low") {
        this->setDecodingType(DecodingType::low);
    } else if (type == "high") {
        this->setDecodingType(DecodingType::high);
    } else {
        this->setDecodingType(DecodingType::low);
    }
    consolelog("SACEffects", LogType::interaction, "decoding type set to " + type);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slot action for menu binaural quality items
 * @param   item                selected item
 */
void SACEffects::toggleBinauralQuality(QAction *item) {
    QObject::sender()->blockSignals(true);
    std::string prefix = "menu_binauralquality_";
    std::string type = item->objectName().toStdString().substr(prefix.length());
    if (type == "normal") {
        this->setBinauralQuality(BinauralQuality::parametric);
    } else if (type == "blind") {
        this->setBinauralQuality(BinauralQuality::filtering);
    } else {
        this->setBinauralQuality(BinauralQuality::parametric);
    }
    consolelog("SACEffects", LogType::interaction,
               "binaural quality set to " + type);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slot action for menu HRTF model items
 * @param   item                selected item
 */
void SACEffects::toggleHRTFModel(QAction *item) {
    QObject::sender()->blockSignals(true);
    std::string prefix = "menu_hrtfmodel_";
    std::string type = item->objectName().toStdString().substr(prefix.length());
    if (type == "kemar") {
        this->setHRTFModel(HRTFModel::kemar);
    } else if (type == "vast") {
        this->setHRTFModel(HRTFModel::vast);
    } else if (type == "mpsvt") {
        this->setHRTFModel(HRTFModel::mps_vt);
    } else {
        this->setHRTFModel(HRTFModel::kemar);
    }
    consolelog("SACEffects", LogType::interaction, "HRTF model set to " + type);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   Slot action for menu effect items
 */
void SACEffects::toggleEffect() {
    QObject::sender()->blockSignals(true);
    std::string effect = QObject::sender()->objectName().toStdString().substr(
                QObject::sender()->objectName().toStdString().find(
                    ui->menu_effect->objectName().toStdString() + "_")
                + ui->menu_effect->objectName().toStdString().length() + 1);
    std::map<Effect::effectID, std::string> effects = this->effectsmonitor->effects;
    for (std::map<Effect::effectID, std::string>::iterator iterator =
         effects.begin(); iterator != effects.end(); iterator++) {
        if (iterator->second == effect) {
            this->setEffect(iterator->first);
            consolelog("SACEffects", LogType::interaction,
                       "selecting effect \"" + iterator->second + "\"");
            QObject::sender()->blockSignals(false);
            return;
        }
    }
    consolelog("SACEffects", LogType::error,
               "effect \"" + effect + "\" is not available");
    QObject::sender()->blockSignals(false);
}

/** @} */

/**
 * @brief   It applies the selected effect to the input signal.
 */
void SACEffects::applyEffect() {
    std::vector<bool> channels;
    std::vector<double> levels;
    for (int index = 0; index < (int) this->channels_input->getSize(); index++) {
        channels.push_back(!this->channels_input->getChannel(index)->bypassed);
        levels.push_back(this->channels_input->getChannel(index)->volume);
    }
    this->process->applyEffect(this->effect, channels, levels);
}
