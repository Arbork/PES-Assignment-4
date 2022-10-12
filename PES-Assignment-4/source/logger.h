/*
 * @file 	logger.h
 * @brief 	Defines the logging method to use
 *
 * @author 	Alexander Bork
 * @date 	Oct 8, 2022
 * @version 1.0
 */

#ifndef LOGGER_H_
#define LOGGER_H_


// Define PRINTF as the logging output if we are debugging
#ifdef DEBUG
	#include <stdio.h>
	#define logger	PRINTF
#else
	#define logger LOG(...)
#endif

#endif /* LOGGER_H_ */
