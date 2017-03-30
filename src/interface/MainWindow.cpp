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
