/*
 * @file 	Timer.c
 * @brief 	Functions to keep track of time in project
 *
 * @author 	Alexander Bork
 * @date 	October 8th, 2022
 * @version 1.0
 */


#include "Timer.h"

static ticktime_t timeSinceReset = 0;
volatile ticktime_t timeSinceBoot = 0;


/*
 * @brief	Initializing the SysTick counter
 *
 * 		Sets the counter value and irq priority then enables Systick
 *
 * @params	None
 * @return	void
 */
void init_systick(void){
	SysTick->LOAD = SYS_COUNTER;
	NVIC_SetPriority(SysTick_IRQn, 3);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}

/*
 * @brief	Returns the time since startup
 *
 * @params 	None
 * @return	timeSinceBoot	- The time since bootup
 */
ticktime_t now(void){
	return timeSinceBoot;
}

/*
 * @brief	Reset the tiemr
 *
 * 		Sets TSR to be TSB so that get_timer returns 0
 * @params	None
 * @return	void
 */
void reset_timer(void){
	timeSinceReset = timeSinceBoot;
}

/*
 * @brief	Get the difference btwn TSB and TSR
 *
 * @params	None
 * @return	The difference in time between TSB and TSR
 */
ticktime_t get_timer(void){
	return (timeSinceBoot - timeSinceReset);
}

/*
 * @brief The interrupt handler for Systick
 *
 * 		Increments time since boot every interrupt
 *
 * @params	None
 * @return	void
 */
void SysTick_Handler(void){
	timeSinceBoot++;
}



