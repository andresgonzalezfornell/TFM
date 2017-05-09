#ifndef LOGGER_H_
#define LOGGER_H_

// System libraries
#include "iostream"
#include "stdlib.h"

/**
 * @name    LogType
 * @brief   Logger message type.
 */
struct LogType {
	enum logtype {
		info, /**< The message is not important, just some information for the user */
		warning, /**< The message is a warning */
		error, /**< The message comes from an bad execution (do not confuse with execution or compilation errors) */
		progress, /**< Information about the current steps in the running execution */
		interaction /**< Information about an user interaction */
	};
};

void consolelog(std::string, LogType::logtype logtype, std::string);

#endif /* LOGGER_H_ */
