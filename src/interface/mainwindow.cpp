/**
 * @name	mainwindow.cpp
 * @author	Andrés González Fornell
 * @brief	Main window interface code.
 */

// Class libraries
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
Input input();

/**
 * @brief	MainWindow constructor.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Input
    input = new Input(ui->input);
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
 * @name	Input
 * @brief	Input control functions.
 * @{
 */

void MainWindow::on_input_device_currentIndexChanged(int index)
{
    consolelog("mainwindow",interaction,"input device set to device[" + std::to_string(index) + "]");
    input->setDevice(index);
}

void MainWindow::on_input_volume_valueChanged(int value)
{
    consolelog("mainwindow",interaction,"input volumen level set to " + std::to_string(value));
    input->setVolume(value);
}

void MainWindow::on_input_pause_clicked()
{
    consolelog("mainwindow",interaction,"input play/pause clicked");
    input->playPause(ui->input_pause);
}

/** @} */
