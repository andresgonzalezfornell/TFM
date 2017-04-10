// Class libraries
#include "Coder.h"

/**
 * @brief	Coder constructor.
 */
Coder::Coder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Coder)
{
    ui->setupUi(this);
    this->fs = 44100;
    this->input = NULL;
    this->output = NULL;
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
    std::string loadingrequest = "please load a file";
    QFont font;
    if(filepath=="") {
        font.setItalic(true);
        framework->setFont(font);
        framework->setText(QString::fromStdString(loadingrequest));
        consolelog("Coder",LogType::warning,"selected audio file is empty");
    } else {
        font.setItalic(false);
        framework->setFont(font);
        framework->setText(QString::fromStdString(filepath));
        consolelog("Coder",LogType::progress,"selected \"" + filepath + "\" as audio file");
    }
    // Buttons enabling
    ui->coding_controls->button(QDialogButtonBox::Apply)->setEnabled(this->input->exists());
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
    if(filepath != "") {
        this->input = new WAVFile(filepath);
    }
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
    if(filepath != "") {
        this->output = new AudioFile(filepath);
    }
    this->setFilename(filepath,ui->output_filename);
    if(filepath != "") {
        // SAC coding
        int bury = 0;
        int treeConfig = 5151;  // tree config: 5151, 5152, 525 (5151 is the default)
        int timeSlots = 32;     // times slots, 16 or 32 (32 is the default)
        std::system("../../lib/SAC/sac_enc/bin/sac_enc");
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
