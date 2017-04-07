// Class libraries
#include "Decoder.h"
#include "ui_Decoder.h"

/**
 * @brief	Decoder constructor.
 */
Decoder::Decoder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Decoder)
{
    ui->setupUi(this);
    // Channels
    this->fs = 44100;
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
    this->fs = fs;
}
