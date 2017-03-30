/**
 * @name	test.cpp
 * @author	Andrés González Fornell
 * @brief	Test Unit file.
 */

// Functional libraries
#include "tools/Logger.h"
#include "module/AudioObject.h"

int main(int argc, char *argv[]) {
	consolelog("test", LogType::progress, "running test");

	consolelog("test", LogType::progress, "test was completed");
	return 0;
}
