// Class libraries
#include "mainwindow.h"
#include "ui_mainwindow.h"

int Objects::fs;

/**
 * @brief	MainWindow constructor.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Objects
    this->setfs(4);
    this->objects = new Objects(ui->objects);
    QObject::connect(ui->objects_fs_0,SIGNAL(triggered(bool)),this,SLOT(setfs()));
    QObject::connect(ui->objects_fs_1,SIGNAL(triggered(bool)),this,SLOT(setfs()));
    QObject::connect(ui->objects_fs_2,SIGNAL(triggered(bool)),this,SLOT(setfs()));
    QObject::connect(ui->objects_fs_3,SIGNAL(triggered(bool)),this,SLOT(setfs()));
    QObject::connect(ui->objects_fs_4,SIGNAL(triggered(bool)),this,SLOT(setfs()));
    consolelog("MainWindow", LogType::progress, "MainWindow object is created");
}

/**
 * @brief	MainWindow destructor.
 *
 */
MainWindow::~MainWindow()
{
    delete ui;
    consolelog("mainwindow", LogType::progress, "MainWindow object is deleted");
}

/**
 * @brief   It sets the signal sampling frequency.
 * @param   index
 */
void MainWindow::setfs(int index) {
    int fs;
    ui->objects_fs_0->setChecked(false);
    ui->objects_fs_1->setChecked(false);
    ui->objects_fs_2->setChecked(false);
    ui->objects_fs_3->setChecked(false);
    ui->objects_fs_4->setChecked(false);
    switch (index) {
    case 0:
        ui->objects_fs_0->setChecked(true);
        fs = 8000;
        break;
    case 1:
        fs = 11025;
        ui->objects_fs_1->setChecked(true);
        break;
    case 2:
        fs = 22050;
        ui->objects_fs_2->setChecked(true);
        break;
    case 3:
        fs = 32000;
        ui->objects_fs_3->setChecked(true);
        break;
    case 4:
    default:
        fs = 44100;
        ui->objects_fs_4->setChecked(true);
    }
    Objects::fs = fs;
    consolelog("MainWindow", LogType::interaction,"signal sampling frequency changed to " + std::to_string(fs));
    this->objects->setfs(fs);
}

/**
 * @brief   It sets the signal sampling frequency.
 */
void MainWindow::setfs() {
    std::string prefix = "objects_fs_";
    int index = std::stoi(QObject::sender()->objectName().toStdString().substr(QObject::sender()->objectName().toStdString().find(prefix) + prefix.length(),1));
    this->setfs(index);
}
