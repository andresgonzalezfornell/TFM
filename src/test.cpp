/**
 * @name	test.cpp
 * @author	Andrés González Fornell
 * @brief	Test Unit file.
 */

// Functional libraries
#include "tools/Logger.h"
#include "process/ProcessManager.h"

#include "fstream"

std::string read(int length, std::fstream *file) {
    if (file->is_open()) {
        std::string data = "";
        for(int byte = 0; byte < length; byte++) {
            char *datapointer = new char[1];
            file->read(datapointer, 1);
            if(std::string(datapointer)=="") {
                data += '\0';
            } else {
                data += std::string(datapointer);
            }
            delete[] datapointer;
        }
        return data;
    }
}

int main(int argc, char *argv[]) {
    consolelog("test", LogType::progress, "running test");
    char s = '\0';
    int length = 4;
    std::string data;
    std::string data1;
    std::string data2;
    std::string data3;
    std::fstream *file = new std::fstream("/Users/andresgonzalezfornell/Desktop/file.txt", std::ios::in | std::ios::binary | std::ios::ate);
    file->seekg(0, std::ios::beg);
    data = read(length, file);
    unsigned long int number = 0;
    for (int byte = 0; byte<length; byte++) {
        number |= data[byte] << (8*(length-byte-1));
    }
    file->close();
    consolelog("test", LogType::progress, "test was completed");
	return 0;
}
