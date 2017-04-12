// Class libraries
#include "Decoder.h"
#include "ui_Decoder.h"

int ChannelsList::fs;

/**
 * @brief	Decoder constructor.
 */
Decoder::Decoder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Decoder)
{
    ui->setupUi(this);
    this->file = NULL;
    QObject::connect(ui->menu_input_load,SIGNAL(triggered(bool)),this,SLOT(load()));
    QObject::connect(ui->menu_input_encode,SIGNAL(triggered(bool)),this,SLOT(encode()));
    QObject::connect(ui->input_start,SIGNAL(released()),this,SLOT(decode()));
    // Channels
    ChannelsList::fs = 44100;
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
 * @brief   It sets the signal sampling frequency.
 */
void Decoder::setfs(int fs) {
    ChannelsList::fs = fs;
}

/**
 * @name    Decoder interface slots
 * @brief   User interface control functions of decoder window.
 * @{
 */

/**
 * @brief   It loads a file as decoder input.
 */
void Decoder::load() {
    std::string filepath = QFileDialog::getOpenFileName(NULL, "Load audio file","","*.wav").toStdString();
    if(filepath=="") {
        consolelog("Decoder",LogType::interaction,"canceling source file selection for decoder");
    } else {
        consolelog("Decoder",LogType::interaction,"selected \"" + filepath + "\" as source file for decoder");
        this->file = new AudioFile(filepath);
    }
}

/**
 * @brief   It encodes a new audio through Coder.
 */
void Decoder::encode() {
    Coder *coder = new Coder();
    coder->exec();
    if (coder->output) {
        this->file = coder->output;
    }
}

/**
 * @brief   It decodes the audio input.
 */
void Decoder::decode() {
    sac_decode();
}

/** @} */
