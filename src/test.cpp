/**
 * @name	test.cpp
 * @author	Andrés González Fornell
 * @brief	Test Unit file.
 */

// Functional libraries
#include "tools/Logger.h"
#include "module/AudioObject.h"

int main(int argc, char *argv[]) {
	consolelog("test", progress, "running test");

	AudioObject object1(2);
    object1.setData(1,1.1);
    consolelog("main",info,std::to_string(object1.getData(1)));

	consolelog("test", progress, "test was completed");
}
