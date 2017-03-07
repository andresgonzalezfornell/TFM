/**
 * @name	mainwindow.cpp
 * @author	Andrés González Fornell
 * @brief	Main window interface code.
 */

#include "../interface/mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    consolelog("mainwindow", progress, "creating MainWindow object");
}

MainWindow::~MainWindow()
{
    delete ui;
    consolelog("mainwindow", progress, "deleting MainWindow object");
}

void MainWindow::on_input_currentIndexChanged(const QString &arg1)
{
    consolelog("mainwindow", progress, "Trying to change input");
}
