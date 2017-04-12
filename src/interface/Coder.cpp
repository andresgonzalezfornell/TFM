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
    this->setInput("");
    this->reset();
    // Signals
    QObject::connect(ui->bitstream_filename,SIGNAL(textEdited(QString)),this,SLOT(setBitstream()));
    QObject::connect(ui->bitstream_buried,SIGNAL(toggled(bool)),this,SLOT(setBuried(bool)));
    QObject::connect(ui->tree_5151,SIGNAL(released()),this,SLOT(toggleTree()));
    QObject::connect(ui->tree_5152,SIGNAL(released()),this,SLOT(toggleTree()));
    QObject::connect(ui->tree_525,SIGNAL(released()),this,SLOT(toggleTree()));
    QObject::connect(ui->input_controls->button(QDialogButtonBox::Open),SIGNAL(released()),this,SLOT(load()));
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
 * @brief   It sets the input audio file.
 * @param   filepath            file path
 */
void Coder::setInput(std::string filepath) {
    if(filepath != "") {
        this->input = new WAVFile(filepath);
    }
    bool loaded = this->input->exists();
    if(loaded && filepath != "") {
        ui->input_filename->setText(QString::fromStdString(filepath));
        consolelog("Coder",LogType::progress,"selected \"" + filepath + "\" as input audio file");
    } else if(loaded && filepath == "") {
        consolelog("Coder",LogType::interaction,"input file selection was cancelled");
    } else {
        ui->input_filename->setText(QString::fromStdString("please load a file"));
        consolelog("Coder",LogType::warning,"input audio file is empty");
    }
    // File name font
    QFont font;
    font.setItalic(!loaded);
    ui->input_filename->setFont(font);
    // Buttons enabling
    ui->coding_controls->button(QDialogButtonBox::Apply)->setEnabled(loaded);
    ui->output_controls->button(QDialogButtonBox::Ok)->setEnabled(loaded && this->output->exists());
}

/**
 * @brief   It sets the output audio file.
 * @param   filepath            file path
 */
void Coder::setOutput(std::string filepath) {
    if(filepath != "") {
        this->output = new WAVFile(filepath);
    }
    bool loaded = this->output->exists();
    if(loaded && filepath != "") {
        ui->output_filename->setText(QString::fromStdString(filepath));
        consolelog("Coder",LogType::progress,"selected \"" + filepath + "\" as output audio file");
    } else if(loaded && filepath == "") {
        consolelog("Coder",LogType::interaction,"output file selection was cancelled");
    } else {
        ui->output_filename->setText(QString::fromStdString("please load a file"));
        consolelog("Coder",LogType::warning,"output audio file is empty");
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
 * @param   index               configuration index
 */
void Coder::setTree(int tree) {
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
 * @param   bitstream           bitstream file name
 */
void Coder::setBitstream() {
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
    consolelog("Coder",LogType::interaction,"bitstream file name changed to " + filename);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   It sets the buried parameter.
 * @param   state              true if it is buried
 */
void Coder::setBuried(bool state) {
    ui->bitstream_buried->setChecked(state);
    ui->bitstream_filename->setEnabled(!state);
    if (state) {
        consolelog("Coder",LogType::interaction,"bitstream is changed to be buried");
    } else {
        consolelog("Coder",LogType::interaction,"bitstream is changed to be a file");
    }
}

/**
 * @brief   It performs change action when tree configuration is changed.
 */
void Coder::toggleTree() {
    QObject::sender()->blockSignals(true);
    std::string prefix = "tree_";
    int tree = std::stoi(QObject::sender()->objectName().toStdString().substr(QObject::sender()->objectName().toStdString().find(prefix) + prefix.length()));
    this->setTree(tree);
    consolelog("Coder",LogType::interaction,"tree configuration changed to " + std::to_string(tree));
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   It loads an input audio file.
 */
void Coder::load() {
    QObject::sender()->blockSignals(true);
    consolelog("Coder",LogType::interaction,"selecting input audio file");
    std::string filepath = QFileDialog::getOpenFileName(NULL, "Load audio file","","*.wav").toStdString();
    this->setInput(filepath);
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   It resets all coding parameters, including output file.
 */
void Coder::reset() {
    this->output = NULL;
    this->setOutput("");
    this->setBuried(false);
    this->setTree(5151);
    ui->bitstream_filename->setText("bitstream.bs");
    consolelog("Coder",LogType::interaction,"all coding parameters has been reset as default");
}

/**
 * @brief   It applies the codification.
 */
void Coder::apply() {
    QObject::sender()->blockSignals(true);
    consolelog("Coder",LogType::interaction,"selecting output audio file");
    std::string input_str = this->input->getFilepath();
    std::string output_str = QFileDialog::getSaveFileName(NULL, "Load audio file","","*.wav").toStdString();
    std::string bitstream_str = output_str.substr(0,output_str.find_last_of("/")+1);
    if(output_str != "") {
        // SAC encoding
        consolelog("Coder",LogType::progress,"proceeding to SAC encoder");
        if(ui->bitstream_buried->isChecked()) {
            bitstream_str = "buried";
        } else {
            bitstream_str += ui->bitstream_filename->text().toStdString();
        }
        consolelog("Coder",LogType::info,"input file:\t " + input_str);
        consolelog("Coder",LogType::info,"output file:\t " + output_str);
        consolelog("Coder",LogType::info,"bitstream file:\t " + bitstream_str);
        const char *input_char = input_str.c_str();
        const char *output_char = output_str.c_str();
        const char *bitstream_char = bitstream_str.c_str();
        int timeslots = 32;
        char *error = sac_encode(input_char,output_char,bitstream_char,this->tree,timeslots,(double)this->fs); // SAC function
        if(error == NULL) {
            this->setOutput(output_str);
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
void Coder::cancel() {
    QObject::sender()->blockSignals(true);
    this->output = NULL;
    consolelog("Coder",LogType::interaction,"the coding has been cancelled");
    QObject::sender()->blockSignals(false);
}

/**
 * @brief   It loads the output to decoder and closes coder dialog.
 */
void Coder::submit() {
    QObject::sender()->blockSignals(true);
    consolelog("Coder",LogType::interaction,"loading output from coder");
    QObject::sender()->blockSignals(false);
}

/** @} */
