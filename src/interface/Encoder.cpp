// Class libraries
#include "Encoder.h"

/**
 * @brief	Encoder constructor.
 */
Encoder::Encoder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Encoder)
{
    ui->setupUi(this);
    this->fs = 44100;
    this->input = NULL;
    this->output = NULL;
    this->bitstream = NULL;
    this->setInput("");
    this->reset();
    // Signals
    QObject::connect(ui->bitstream_filename,SIGNAL(textEdited(QString)),this,SLOT(setBitstream()));
    QObject::connect(ui->bitstream_buried,SIGNAL(toggled(bool)),this,SLOT(setBuried(bool)));
    QObject::connect(ui->tree_5151,SIGNAL(released()),this,SLOT(toggleTree()));
    QObject::connect(ui->tree_5152,SIGNAL(released()),this,SLOT(toggleTree()));
    QObject::connect(ui->tree_525,SIGNAL(released()),this,SLOT(toggleTree()));
    QObject::connect(ui->input_controls->button(QDialogButtonBox::Open),SIGNAL(released()),this,SLOT(load()));
    QObject::connect(ui->encoding_controls->button(QDialogButtonBox::Reset),SIGNAL(released()),this,SLOT(reset()));
    QObject::connect(ui->encoding_controls->button(QDialogButtonBox::Apply),SIGNAL(released()),this,SLOT(apply()));
    QObject::connect(ui->output_controls->button(QDialogButtonBox::Cancel),SIGNAL(released()),this,SLOT(cancel()));
    QObject::connect(ui->output_controls->button(QDialogButtonBox::Ok),SIGNAL(released()),this,SLOT(submit()));
    consolelog("Encoder", LogType::progress, "Encoder object is created");
}

/**
 * @brief	Encoder destructor.
 *
 */
Encoder::~Encoder()
{
    delete ui;
    consolelog("Encoder", LogType::progress, "Encoder object is deleted");
}

/**
 * @brief   It sets the input audio file.
 * @param   filename            file path
 */
void Encoder::setInput(std::string filename) {
    if(filename != "") {
        this->input = new WAVFile(filename);
    }
    bool loaded = this->input->exists();
    if(loaded && filename != "") {
        ui->input_filename->setText(QString::fromStdString(filename));
        consolelog("Encoder",LogType::progress,"selected \"" + filename + "\" as input audio file");
    } else if(loaded && filename == "") {
        ui->input_filename->setText(QString::fromStdString(this->input->getFilename()));
        consolelog("Encoder",LogType::interaction,"input file selection was cancelled");
    } else {
        ui->input_filename->setText(QString::fromStdString("please load a file"));
        consolelog("Encoder",LogType::warning,"input audio file is empty");
    }
    // File name font
    QFont font;
    font.setItalic(!loaded);
    ui->input_filename->setFont(font);
    // Buttons enabling
    ui->encoding_controls->button(QDialogButtonBox::Apply)->setEnabled(loaded);
    ui->output_controls->button(QDialogButtonBox::Ok)->setEnabled(loaded && this->output->exists());
}

/**
 * @brief   It sets the output audio file.
 * @param   filename            file path
 */
void Encoder::setOutput(std::string filename) {
    if(filename != "") {
        this->output = new WAVFile(filename);
    }
    bool loaded = this->output->exists();
    if(loaded && filename != "") {
        ui->output_filename->setText(QString::fromStdString(filename));
        consolelog("Encoder",LogType::progress,"selected \"" + filename + "\" as output audio file");
    } else if(loaded && filename == "") {
        ui->output_filename->setText(QString::fromStdString(this->output->getFilename()));
        consolelog("Encoder",LogType::interaction,"output file selection was cancelled");
    } else {
        ui->output_filename->setText(QString::fromStdString("please load a file"));
        consolelog("Encoder",LogType::warning,"output audio file is empty");
    }
    // File name font
    QFont font;
    font.setItalic(!loaded);
    ui->output_filename->setFont(font);
    // Buttons enabling
    ui->output_controls->button(QDialogButtonBox::Ok)->setEnabled(loaded);
}

/**
 * @brief   It sets a tree configuration.
 * @param   tree                configuration index
 */
void Encoder::setTree(int tree) {
    this->tree = tree;
    ui->tree_5151->setChecked(false);
    ui->tree_5152->setChecked(false);
    ui->tree_525->setChecked(false);
    ui->tree_group->findChild<QRadioButton *>(QString("tree_") + QString::number(tree))->setChecked(true);
}

/**
 * @name    Coder interface slots
 * @brief   User interface control functions of coder.
 * @{
 */

/**
 * @brief   It sets the bitstream file name.
 */
void Encoder::setBitstream() {
    QObject::sender()->blockSignals(true);
    std::string filename = ui->bitstream_filename->text().toStdString();
    // Deleting reserved characters
    char reservedchar[6] = {'/','>','<','|',':','&'};
    for(int index = 0; index < (int)sizeof(reservedchar); index++) {
        while(filename.find(reservedchar[index]) < filename.length()) {
            filename = filename.substr(0,filename.find(reservedchar[index])) + filename.substr(filename.find(reservedchar[index])+1);
        }
    }
    ui->bitstream_filename->setText(QString::fromStdString(filename));
    consolelog("Encoder",LogType::interaction,"bitstream file name changed to " + filename);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   It sets the buried parameter.
 * @param   state              true if it is buried
 */
void Encoder::setBuried(bool state) {
    ui->bitstream_buried->setChecked(state);
    ui->bitstream_filename->setEnabled(!state);
    if (state) {
        consolelog("Encoder",LogType::interaction,"bitstream is changed to be buried");
    } else {
        consolelog("Encoder",LogType::interaction,"bitstream is changed to be a file");
    }
}

/**
 * @brief   It performs change action when tree configuration is changed.
 */
void Encoder::toggleTree() {
    QObject::sender()->blockSignals(true);
    std::string prefix = "tree_";
    int tree = std::stoi(QObject::sender()->objectName().toStdString().substr(QObject::sender()->objectName().toStdString().find(prefix) + prefix.length()));
    this->setTree(tree);
    consolelog("Encoder",LogType::interaction,"tree configuration changed to " + std::to_string(tree));
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   It loads an input audio file.
 */
void Encoder::load() {
    QObject::sender()->blockSignals(true);
    consolelog("Encoder",LogType::interaction,"selecting input audio file");
    std::string filename = QFileDialog::getOpenFileName(NULL, "Load input audio file for encoder","","*.wav").toStdString();
    this->setInput(filename);
    if (filename != "") {
        this->reset();
    }
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   It resets all encoding parameters, including output file.
 */
void Encoder::reset() {
    // Encoding
    ui->bitstream_filename->setText("bitstream.bs");
    this->setBuried(false);
    this->setTree(5151);
    // Output
    this->output = NULL;
    this->setOutput("");
    consolelog("Encoder",LogType::interaction,"all encoding parameters has been reset as default");
}

/**
 * @brief   It applies the codification.
 */
void Encoder::apply() {
    QObject::sender()->blockSignals(true);
    consolelog("Encoder",LogType::interaction,"encoding");
    std::string input_str = this->input->getFilename();
    std::string output_str = QFileDialog::getSaveFileName(NULL, "Save output file for encoder","","*.wav").toStdString();
    std::string bitstream_str = output_str.substr(0,output_str.find_last_of("/")+1);
    if(output_str != "") {
        // SAC encoding
        consolelog("Encoder",LogType::progress,"proceeding to SAC encoder");
        if(ui->bitstream_buried->isChecked()) {
            bitstream_str = "buried";
        } else {
            bitstream_str += ui->bitstream_filename->text().toStdString();
        }
        const char *input_char = input_str.c_str();
        const char *bitstream_char = bitstream_str.c_str();
        const char *output_char = output_str.c_str();
        int timeslots = this->input->header.bitspersample;
        consolelog("Encoder",LogType::info,"input file:\t " + input_str);
        consolelog("Encoder",LogType::info,"bitstream file:\t " + bitstream_str);
        consolelog("Encoder",LogType::info,"output file:\t " + output_str);
        consolelog("Encoder",LogType::info,"sampling frequency:\t " + std::to_string(this->fs) + "Hz");
        consolelog("Encoder",LogType::info,"tree configuration:\t " + std::to_string(this->tree));
        consolelog("Encoder",LogType::info,"time slots:\t " + std::to_string(timeslots) + "bits");
        char *error = sac_encode(input_char,output_char,bitstream_char,(double)this->fs,this->tree,timeslots); // SAC function
        if(error == NULL) {
            this->setOutput(output_str);
            if(bitstream_str == "buried") {
                this->bitstream = NULL;
            } else {
                this->bitstream = new File(bitstream_str);
            }
            consolelog("sac_encoder",LogType::progress,"encoding was completed successfully");
        } else {
            consolelog("sac_encoder",LogType::error,std::string(error));
        }
    }
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   It closes the coder dialog without no consequences.
 */
void Encoder::cancel() {
    QObject::sender()->blockSignals(true);
    this->output = NULL;
    this->bitstream = NULL;
    consolelog("Encoder",LogType::interaction,"the coding has been cancelled");
    QObject::sender()->blockSignals(false);
    this->close();
}

/**
 * @brief   It loads the output to decoder and closes coder dialog.
 */
void Encoder::submit() {
    QObject::sender()->blockSignals(true);
    consolelog("Encoder",LogType::interaction,"loading output from coder");
    QObject::sender()->blockSignals(false);
    this->close();
}

/** @} */
