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
#include "audiochart.h"
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
private slots:
    void on_device_selector_currentIndexChanged(int index);
    void on_device_volume_valueChanged(int value);
    void on_device_muting_clicked();
    void on_objects_number_valueChanged(int value);
private:
    Ui::MainWindow *ui;     /**< User interface object */
    Objects *objects;       /**< Objects panel object */
};

#endif // MAINWINDOW_H
