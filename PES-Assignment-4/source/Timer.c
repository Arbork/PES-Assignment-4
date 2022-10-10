/*
 * Timer.c
 *
 *  Created on: Oct 9, 2022
 *      Author: zande
 */


#include "Timer.h"

static ticktime_t timeSinceReset = 0;
static ticktime_t timeSinceBoot = 0;


/*
 *
 */
void init_systick(void){
	SysTick->LOAD = SYS_COUNTER;
	NVIC_SetPriority(SysTick_IRQn, 3);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
}

/*
 *
 */
ticktime_t now(void){
	return timeSinceBoot;
}

/*
 *
 */
void reset_timer(void){
	timeSinceReset = timeSinceBoot;
}

/*
 *
 */
ticktime_t get_timer(void){
	return (timeSinceBoot - timeSinceReset);
}

void SysTick_Handler(void){
	timeSinceBoot++;
}


