/**
 * @name	main.cpp
 * @author	Andrés González Fornell
 * @brief	Main function
 */

// Qt libraries
#include "QApplication"
#include "QMainWindow"
// Classes and local files
#include "Encoder.h"
#include "Decoder.h"
#include "../tools/Logger.h"

/**
 * @brief	Main function
 */
int main(int argc, char *argv[]) {
    consolelog("main", LogType::progress, "application program");
    QApplication app(argc, argv);
    Decoder decoder;
    consolelog("main", LogType::progress, "showing Decoder");
    decoder.show();
    consolelog("main", LogType::progress, "application was completed");
    return app.exec();
}
