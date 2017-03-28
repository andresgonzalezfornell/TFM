// Class libraries
#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief	MainWindow constructor.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Objects
    objects = new Objects(ui->objects);
    consolelog("mainwindow", LogType::progress, "MainWindow object is created");
}

/**
 * @brief	MainWindow destructor.
 */
MainWindow::~MainWindow()
{
    delete ui;
    consolelog("mainwindow", LogType::progress, "MainWindow object is deleted");
}

/**
 * @name	Objects
 * @brief	Objects control functions.
 * @{
 */
void MainWindow::on_device_selector_currentIndexChanged(int index)
{
    consolelog("mainwindow",LogType::interaction,"input device set to device[" + std::to_string(index) + "]");
    objects->device->setDevice(index);
}

void MainWindow::on_device_volume_valueChanged(int value)
{
    consolelog("mainwindow",LogType::interaction,"device volumen level set to " + std::to_string(value));
    objects->device->setVolume(value);
}

void MainWindow::on_device_muting_clicked()
{
    consolelog("mainwindow",LogType::interaction,"device play/pause clicked");
    objects->device->switchMuting();
}

void MainWindow::on_objects_number_valueChanged(int value) {
    consolelog("mainwindow",LogType::interaction,"objects number changed to " + std::to_string(value));
    objects->objectsconfiguration->setNumber(value);
    //update();
}

/** @} */
