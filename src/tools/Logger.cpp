/**
 * @name	Logger.cpp
 * @author	Andrés González Fornell
 * @brief	Functions to create log messages on console.
 */

// Class libraries
#include "Logger.h"

using namespace std;

/**
 * @name	Font styles
 * @brief	ANSI code for some font styles for log messages usage.
 * @{
 */
const string reset = "\033[0m";
const string bold = "\033[1m";
const string italic = "\033[3m";
const string black = "\033[30m";
const string red	 = "\033[31m";
const string green = "\033[32m";
const string yellow = "\033[33m";
const string blue = "\033[34m";
const string magenta = "\033[35m";
const string cyan = "\033[36m";
const string white = "\033[37m";
/** @} */

/**
 * @brief	It returns the string tab code to align log messages.
 * @param	content		Content of the tabulation
 * @param	tab_max		Maximum number of tabulations
 */
string tab(string content, const int tab_max) {
	const int spacespertab = 8;
	int tab_number = 0;
	string tab = "";
	tab_number = tab_max - int(content.length() / spacespertab);
	for (int i = 1; i <= tab_number; i++) {
		tab = tab + "\t";
	}
	return tab;
}

/**
 * @brief	Log a message on console
 * @param	source		origin class/method/file where the message was logged
 * @param	type		type of message
 * @param	message		message
 * @return	void
 */
void consolelog(string source, logtype type, string message) {
	const int tab_max = 2;
	const int spacespertab = 8;
	// Source
	if (source.length() >= tab_max * spacespertab) {
		source = source.substr(0, 15);
	}
	// Log type
	string type_name = "";
	string type_style = "";
	switch (type) {
	case info:
		type_name = "info";
		type_style = blue;
		break;
	case warning:
		type_name = "warning";
		type_style = yellow;
		break;
	case error:
		type_name = "error";
		type_style = red;
		break;
	case progress:
		type_name = "progress";
		type_style = green;
		break;
	case interaction:
		type_name = "user action";
		type_style = magenta;
		break;
	default:
		type_name = "general";
		type_style = cyan;
		break;
	}
	// Log
	cout << white << "Log message ";
	cout << "from " << italic << source << tab(source, tab_max) << reset;
	cout << bold << type_style << type_name << tab(type_name, tab_max) << reset;
	cout << message << reset << endl;
}
