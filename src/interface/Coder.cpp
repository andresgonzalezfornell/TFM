// Class libraries
#include "Coder.h"
#include "ui_Coder.h"

int Channels::fs;

/**
 * @brief	Coder constructor.
 */
Coder::Coder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Coder)
{
    ui->setupUi(this);
    // Channels
    this->setfs(4);
    this->channels = new Channels(ui->channels);
    QObject::connect(ui->channels_fs_0,SIGNAL(triggered(bool)),this,SLOT(setfs()));
    QObject::connect(ui->channels_fs_1,SIGNAL(triggered(bool)),this,SLOT(setfs()));
    QObject::connect(ui->channels_fs_2,SIGNAL(triggered(bool)),this,SLOT(setfs()));
    QObject::connect(ui->channels_fs_3,SIGNAL(triggered(bool)),this,SLOT(setfs()));
    QObject::connect(ui->channels_fs_4,SIGNAL(triggered(bool)),this,SLOT(setfs()));
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
 * @brief   It sets the signal sampling frequency.
 * @param   index
 */
void Coder::setfs(int index) {
    int fs;
    ui->channels_fs_0->setChecked(false);
    ui->channels_fs_1->setChecked(false);
    ui->channels_fs_2->setChecked(false);
    ui->channels_fs_3->setChecked(false);
    ui->channels_fs_4->setChecked(false);
    switch (index) {
    case 0:
        ui->channels_fs_0->setChecked(true);
        fs = 8000;
        break;
    case 1:
        fs = 11025;
        ui->channels_fs_1->setChecked(true);
        break;
    case 2:
        fs = 22050;
        ui->channels_fs_2->setChecked(true);
        break;
    case 3:
        fs = 32000;
        ui->channels_fs_3->setChecked(true);
        break;
    case 4:
    default:
        fs = 44100;
        ui->channels_fs_4->setChecked(true);
    }
    Channels::fs = fs;
    consolelog("Coder", LogType::interaction,"signal sampling frequency changed to " + std::to_string(fs));
    this->channels->setfs(fs);
}

/**
 * @brief   It sets the signal sampling frequency.
 */
void Coder::setfs() {
    std::string prefix = "channels_fs_";
    int index = std::stoi(QObject::sender()->objectName().toStdString().substr(QObject::sender()->objectName().toStdString().find(prefix) + prefix.length(),1));
    this->setfs(index);
}
