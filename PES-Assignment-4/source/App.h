/*
 * App.h
 *
 *  Created on: Oct 9, 2022
 *      Author: zande
 */

#ifndef APP_H_
#define APP_H_

#include "LED.h"
#include "Touch.h"
#include "Timer.h"
#include "MKL25Z4.h"

#define STOP_STATE		0x1
#define GO_STATE		0x2
#define WARN_STATE		0x4
#define CROSS_STATE		0x8
#define FLAG_MASK		0xf
#define false			0
#define true			1

#define ms_ticks			5300
#define PWM_MAX				0xff
#ifdef DEBUG
	#define STATE_DELAY		320
#else
	#define	STATE_DELAY 	80
#endif

typedef enum{
	STOP = 0x1,
	GO = 0x2,
	WARNING = 0x4,
	CROSS = 0x8
} lightState_t;



/*
 * @brief	This function delays the program without being optimized out
 *
 * 		This function creates a delay in milliseconds as specified by the input parameter.
 * 	We stay in a while loop until the counter reaches 0. Since there is not a 1 to 1 relation
 * 	with the number of iterations and milliseconds, we need to multiply milliseconds by ms_ticks
 * 	(which is how many iterations there are per milliseconds) so that the loop lasts for
 * 	the needed amount of milliseconds
 *
 * @param 	milliseconds	The number of milliseconds to delay for
 * @return	void
 */
void delay(uint32_t milliseconds);

void app_init(void);

void GO_transition(void);

void STOP_transition(void);

void WARN_transition(void);

void cross_handler(void);

#endif /* APP_H_ */
