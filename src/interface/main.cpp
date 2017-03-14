/**
 * @name	main.cpp
 * @author	Andrés González Fornell
 * @brief	Main function
 */

// Functional libraries
#include "iostream"
#include "cmath"
#include "stdio.h"
#include "string"
#include "vector"

// Qt libraries
#include "QApplication"
#include "QMainWindow"
// Classes and local files
#include "../tools/Logger.h"
#include "mainwindow.h"


#include "../module/AudioObject.h"

/**
 * @brief	Main function
 */
int main(int argc, char *argv[]) {
    consolelog("main", progress, "running program");
	QApplication a(argc, argv);
    MainWindow w;
    consolelog("main", progress, "showing MainWindow");
    w.show();
    consolelog("main", progress, "program was completed");
	return a.exec();
}
