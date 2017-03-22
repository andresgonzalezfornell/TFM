/**
 * @name	mainwindow.cpp
 * @author	Andrés González Fornell
 * @brief	Main window interface code.
 */

// Class libraries
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

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
    consolelog("mainwindow", progress, "MainWindow object is created");
}

/**
 * @brief	MainWindow destructor.
 */
MainWindow::~MainWindow()
{
    delete ui;
    consolelog("mainwindow", progress, "MainWindow object is deleted");
}

/**
 * @name	Objects
 * @brief	Objects control functions.
 * @{
 */

void MainWindow::on_device_selector_currentIndexChanged(int index)
{
    consolelog("mainwindow",interaction,"input device set to device[" + std::to_string(index) + "]");
    objects->device->setDevice(index);
}

void MainWindow::on_device_volume_valueChanged(int value)
{
    consolelog("mainwindow",interaction,"device volumen level set to " + std::to_string(value));
    objects->device->setVolume(value);
}

void MainWindow::on_device_pause_clicked()
{
    consolelog("mainwindow",interaction,"device play/pause clicked");
    objects->device->playPause(ui->device_pause);
}

void MainWindow::on_objects_number_valueChanged(int value) {
    consolelog("mainwindow",interaction,"objects number changed to " + std::to_string(value));
    objects->objectconfiguration->setNumber(ui->objects_configuration, value);
    //update();
}

/** @} */
