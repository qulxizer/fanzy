/*
 * logger.h
 *
 *  Created on: Jan 16, 2026
 *      Author: Mahdi
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include <stdint.h>


/* ---------------------- Error levels ---------------------- */
typedef enum {
    ERR_ERROR = 0,
    ERR_WARN,
    ERR_INFO,
} err_level_t;

/* ---------------------- Function declarations ---------------------- */
const char* err_level_string(err_level_t lvl);

void log_error(char *msg);
void log_warn(char *msg);
void log_info(char *msg);

/* Internal logging function (used by above) */
void write_log(const char* msg, err_level_t lvl);



#endif /* INC_LOGGER_H_ */
