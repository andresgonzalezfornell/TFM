/**
 * @name	main.cpp
 * @author	Andrés González Fornell
 * @brief	Main function
 */

// Qt libraries
#include "QApplication"
#include "QMainWindow"
// Classes and local files
#include "Coder.h"
#include "Decoder.h"
#include "../tools/Logger.h"

/**
 * @brief	Main function
 */
int main(int argc, char *argv[]) {
    consolelog("main", LogType::progress, "running program");
    QApplication app(argc, argv);
    Decoder decoder;
    consolelog("main", LogType::progress, "showing Decoder");
    decoder.show();
    consolelog("main", LogType::progress, "program was completed");
    return app.exec();
}
