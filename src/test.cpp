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

	std::vector<float> a = {1,2};
	AudioObject object1 = AudioObject(a);
	std::vector<float> w = object1.getData();
	consolelog("test",info,std::to_string(w.at(1)));

	consolelog("test", progress, "test was completed");
}
