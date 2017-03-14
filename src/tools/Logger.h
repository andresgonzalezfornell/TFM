#ifndef LOGGER_H_
#define LOGGER_H_

// System libraries
#include "iostream"
#include "cmath"
#include "stdio.h"
#include "string"

using namespace std;

enum logtype {
    info, warning, error, progress, interaction
};

void consolelog(string, logtype, string);

#endif /* LOGGER_H_ */
