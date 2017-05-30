/**
 * @file	Logger.cpp
 * @author	Andrés González Fornell
 * @brief	Functions to create log messages on console.
 */

// Class libraries
#include "Logger.h"

/**
 * @name	Font styles
 * @brief	ANSI code for some font styles for log messages usage.
 * @{
 */
const std::string reset = "\033[0m"; /**< default style */
const std::string bold = "\033[1m"; /**< bold */
const std::string italic = "\033[3m"; /**< italic */
const std::string black = "\033[30m"; /**< black color font */
const std::string red = "\033[31m"; /**< red color font */
const std::string green = "\033[32m"; /**< green color font */
const std::string yellow = "\033[33m"; /**< yellow color font */
const std::string blue = "\033[34m"; /**< blue color font */
const std::string magenta = "\033[35m"; /**< magenta color font */
const std::string cyan = "\033[36m"; /**< cyan color font */
const std::string grey = "\033[37m"; /**< grey color font */
/** @} */

/**
 * @brief	It returns the string tab code to align log messages.
 * @param	content		Content of the tabulation
 * @param	tab_max		Maximum number of tabulations
 */
std::string tab(std::string content, const int tab_max) {
    const int spacespertab = 8;
    int tab_number = 0;
    std::string tab = "";
    tab_number = tab_max - int(content.length() / spacespertab);
    for (int i = 1; i <= tab_number; i++) {
        tab = tab + "\t";
    }
    return tab;
}

/**
 * @brief	Log a message on console
 * @param	source		origin class/method/file where the message was logged
 * @param	logtype		type of message
 * @param	message		message
 * @return	void
 */
void consolelog(std::string source, LogType::logtype logtype,
                std::string message) {
    const int tab_max = 2;
    const int spacespertab = 8;
    // Source
    if (source.length() >= tab_max * spacespertab) {
        source = source.substr(0, 15);
    }
    // Log type
    std::string type_name = "";
    std::string type_style = "";
    switch (logtype) {
    case LogType::info:
        type_name = "info";
        type_style = blue;
        break;
    case LogType::warning:
        type_name = "warning";
        type_style = yellow;
        break;
    case LogType::error:
        type_name = "error";
        type_style = red;
        break;
    case LogType::progress:
        type_name = "progress";
        type_style = green;
        break;
    case LogType::interaction:
        type_name = "user action";
        type_style = magenta;
        break;
    default:
        type_name = "general";
        type_style = cyan;
        break;
    }
    // Log
    std::cout << grey << "Log message ";
    std::cout << "from " << italic << source << tab(source, tab_max) << reset;
    std::cout << bold << type_style << type_name << tab(type_name, tab_max)
              << reset;
    std::cout << message << reset << std::endl;
}
