/*
 * project_settings.h
 *
 *  Created on: Mar 25, 2019
 *      Author: lonnie2
 */

#ifndef PROJECT_SETTINGS_H_
#define PROJECT_SETTINGS_H_

#include "library.h"

#define FCPU 16000000L

#define USE_MODULE_TASK
#define USE_MODULE_TIMING
#define USE_MODULE_LIST
#define USE_MODULE_BUFFER
#define USE_MODULE_BUFFER_PRINTF
#define USE_MODULE_SUBSYSTEM
//#define SUBSYSTEM_IO SUBSYSTEM_IO_PRINTF

#define USE_MODULE_UART
#define UART0_TX_BUFFER_LENGTH 512
#define USE_UART1
#define SUBSYSTEM_IO SUBSYSTEM_IO_UART
#define SUBSYSTEM_UART 0

#define TASK_MAX_LENGTH 50

#endif /* PROJECT_SETTINGS_H_ */
