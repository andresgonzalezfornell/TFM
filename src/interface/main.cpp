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
#include "SACEffects.h"
#include "../tools/Logger.h"

/**
 * @brief	Main function
 */
int main(int argc, char *argv[]) {
    consolelog("main", LogType::progress, "starting SAC Effects");
    QApplication app(argc, argv);
    SACEffects window;
    consolelog("main", LogType::progress, "showing SAC Effects");
    window.show();
    return app.exec();
}
