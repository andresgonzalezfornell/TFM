#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt libraries
#include "qtlibraries.h"
#include "iostream"
#include "stdio.h"
#include "string"
// Classes and local files
#include "../tools/Logger.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_input_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
