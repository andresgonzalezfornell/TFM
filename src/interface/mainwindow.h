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
#include "AudioChart.h"
#include "device.h"
#include "objects.h"

namespace Ui {
class MainWindow;
}

/**
 * @class	MainWindow
 * @author	Andrés González Fornell
 * @brief	Main window interface code.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initialize();
private:
    Ui::MainWindow *ui;     /**< User interface object */
    Objects *objects;       /**< Objects panel object */
    void setfs(int index);
private slots:
    void setfs();
};

#endif // MAINWINDOW_H
