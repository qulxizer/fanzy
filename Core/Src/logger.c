/*
 * logger.c
 *
 *  Created on: Jan 16, 2026
 *      Author: Mahdi
 */

#include "logger.h"
#include <stdio.h>


/* ---------------------- Error level string mapping ---------------------- */
const char* err_level_string(err_level_t lvl) {
    switch(lvl) {
        case ERR_ERROR: return "ERROR";
        case ERR_WARN:  return "WARN";
        case ERR_INFO:  return "INFO";
        default:        return "UNKNOWN";
    }
}

/* ---------------------- Logging helpers ---------------------- */
void log_error(char *msg) {
    write_log(msg, ERR_ERROR);
}

void log_warn(char *msg) {
    write_log(msg, ERR_WARN);
}

void log_info(char *msg) {
    write_log(msg, ERR_INFO);
}

/* ---------------------- Core logging function ---------------------- */
void write_log(const char* msg, err_level_t lvl) {
    printf("[%s] %s\r\n", err_level_string(lvl), msg);
}
