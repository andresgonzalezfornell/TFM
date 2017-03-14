#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// C libraries
#include "iostream"
#include "stdio.h"
#include "string"
// Qt libraries
#include "QApplication"
#include "QMainWindow"
// Classes and local files
#include "../tools/Logger.h"
#include "input.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initialize();

private slots:
    void on_input_device_currentIndexChanged(int index);
    void on_input_volume_valueChanged(int value);
    void on_input_pause_clicked();

private:
    Ui::MainWindow *ui;
    Input *input;
};

#endif // MAINWINDOW_H
