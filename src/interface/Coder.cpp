// Class libraries
#include "Coder.h"
#include "ui_Coder.h"
#include "../coder/sac_enc.h"
#include "../coder/bitstream.h"
#include "../coder/sac_bd_embedder.h"

int ChannelsConfiguration::fs;

/**
 * @brief	Coder constructor.
 */
Coder::Coder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Coder)
{
    ui->setupUi(this);
    this->fs = 44100;
    this->setFilename("",ui->input_filename);
    this->setFilename("",ui->output_filename);
    // Signals
    QObject::connect(ui->input_controls->button(QDialogButtonBox::Open),SIGNAL(released()),this,SLOT(loadInput()));
    QObject::connect(ui->coding_controls->button(QDialogButtonBox::Reset),SIGNAL(released()),this,SLOT(reset()));
    QObject::connect(ui->coding_controls->button(QDialogButtonBox::Apply),SIGNAL(released()),this,SLOT(apply()));
    QObject::connect(ui->output_controls->button(QDialogButtonBox::Cancel),SIGNAL(released()),this,SLOT(cancel()));
    QObject::connect(ui->output_controls->button(QDialogButtonBox::Ok),SIGNAL(released()),this,SLOT(submit()));
    consolelog("Coder", LogType::progress, "Coder object is created");
}

/**
 * @brief	Coder destructor.
 *
 */
Coder::~Coder()
{
    delete ui;
    consolelog("Coder", LogType::progress, "Coder object is deleted");
}

/**
 * @brief   It sets the file name on the user interface.
 * @param   filepath            file path
 * @param   framework           user interface element to place file path
 */
void Coder::setFilename(std::string filepath, QLineEdit *framework) {
    QFont font;
    if(filepath=="") {
        font.setItalic(true);
        framework->setFont(font);
        framework->setText("please load a file");
        consolelog("Coder",LogType::warning,"selected audio file is empty");
    } else {
        font.setItalic(false);
        framework->setFont(font);
        framework->setText(QString::fromStdString(filepath));
        consolelog("Coder",LogType::progress,"selected \"" + filepath + "\" as audio file");
    }
    ui->output_controls->button(QDialogButtonBox::Ok)->setEnabled(this->input->exists() && this->output->exists());
}

/**
 * @name    Coder interface slots
 * @brief   User interface control functions of coder.
 * @{
 */

/**
 * @brief   It loads an input audio file.
 */
void Coder::loadInput() {
    consolelog("Coder",LogType::interaction,"selecting input audio file");
    std::string filepath = QFileDialog::getOpenFileName(NULL, "Load audio file","","*.wav").toStdString();
    this->setFilename(filepath, ui->input_filename);
}

/**
 * @brief   It resets all coding parameters.
 */
void Coder::reset() {
    consolelog("Coder",LogType::interaction,"all coding parameters has been reset as default");
}

/**
 * @brief   It applies the codification.
 */
void Coder::apply() {
    consolelog("Coder",LogType::interaction,"selecting input audio file");
    std::string filepath = QFileDialog::getSaveFileName(NULL, "Load audio file","","*.sac").toStdString();
    this->setFilename(filepath,ui->output_filename);
    if(filepath != "") {
        this->output = new AudioFile(filepath);
        // SAC coding
        Stream bitstream;
        int bury = 0;
        int treeConfig = 5151;  // tree config: 5151, 5152, 525 (5151 is the default)
        int timeSlots = 32;     // times slots, 16 or 32 (32 is the default)
        long int sampleNum, channelNum;
        double sampleFreq = (double)this->fs;
        int outputchannels = 0;
        switch(treeConfig) {
        case 5151:
        case 5152:
            outputchannels=1;
            break;
        case 525:
            outputchannels=2;
        }
        AFILE *inputfile = AFopnRead(this->input->getFilepath().c_str(), &sampleNum, &channelNum, &sampleFreq, NULL);
        AFILE *outputfile = AFopnWrite(this->output->getFilepath().c_str(), 2+16, 5 , outputchannels, sampleFreq, NULL);
        consolelog("Coder",LogType::interaction,"starting coding");
    //    this->encode(inputfile, 1, outputfile, &bitstream, 1,1,1);
    }
}

/**
 * @brief   It closes the coder dialog without no consequences.
 */
void Coder::cancel() {
    consolelog("Coder",LogType::interaction,"the coding has been cancelled");
}

/**
 * @brief   It loads the output to decoder and closes coder dialog.
 */
void Coder::submit() {
    consolelog("Coder",LogType::interaction,"loading output from coder");
}

/** @} */

static void encode(AFILE *inputFile, int nSamples, AFILE *downmixFile, Stream *bitstream, int bury, int treeConfig, int timeSlots) {

}
