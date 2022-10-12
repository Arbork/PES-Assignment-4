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

#define ms_ticks				5300
#define PWM_MAX					0xff
#define STATE_TRANSITION		16
#define DELAY_65MS				1
#define TOUCH_NOISE_THRESHOLD	50
#define CROSS_DELAY				160
#define CROSS_OFF_TICKS			4
#define CROSS_ON_TICKS			12
#define TOUCH_POLL_MASK			0b1


#ifndef DEBUG
	#define STATE_DELAY		320
	#define WARN_DELAY		80
#else
	#define	STATE_DELAY 	80
	#define WARN_DELAY		48
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
void delay(uint32_t systicks);

void transition(uint8_t final_red, uint8_t final_green, uint8_t final_blue);

void app_init(void);

void app_loop(void);

void cross_handler(void);

#endif /* APP_H_ */
