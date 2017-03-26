/**
 * @name	main.cpp
 * @author	Andrés González Fornell
 * @brief	Main function
 */

// Qt libraries
#include "QApplication"
#include "QMainWindow"
// Classes and local files
#include "mainwindow.h"
#include "../tools/Logger.h"
#include "../module/AudioObject.h"

/**
 * @brief	Main function
 */
int main(int argc, char *argv[]) {
    consolelog("main", LogType::progress, "running program");
	QApplication a(argc, argv);
    MainWindow w;
    consolelog("main", LogType::progress, "showing MainWindow");
    w.show();
    consolelog("main", LogType::progress, "program was completed");
	return a.exec();
}
