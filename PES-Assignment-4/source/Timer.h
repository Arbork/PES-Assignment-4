/*
 * Timer.h
 *
 *  Created on: Oct 9, 2022
 *      Author: zande
 */

#ifndef TIMER_H_
#define TIMER_H_


#include "MKL25Z4.h"
#include "stdio.h"
#include "stdint.h"
#include "core_cm0plus.h"

typedef uint32_t 	ticktime_t;

#define INPUT_FREQ		48000000UL
#define OUTPUT_FREQ		16
#define	SYS_COUNTER		(INPUT_FREQ / (16 * OUTPUT_FREQ))



// Function definitions
/*
 *
 */
void init_systick(void);

/*
 *
 */
ticktime_t now(void);

/*
 *
 */
void reset_timer(void);

/*
 *
 */
ticktime_t get_timer(void);


void set_LED_PWM(uint8_t Red, uint8_t Green, uint8_t Blue);
void SysTick_Handler(void);


#endif /* TIMER_H_ */
