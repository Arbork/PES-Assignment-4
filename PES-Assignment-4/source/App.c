/*
 * App.c
 *
 *  Created on: Oct 9, 2022
 *      Author: zande
 */


#include "App.h"

static LED_colors_t colors;
static lightState_t state;
static uint8_t 		cross;

void app_init(void){
	cross = false;
	state = STOP;
	colors.R = STOP_RED;
	colors.G = STOP_GREEN;
	colors.B = STOP_BLUE;
	LED_init();
	touch_setup();
}

void app_loop(void){
	ticktime_t start_time = 0;
	ticktime_t elapsed_time = 0;
	init_systick();
	while(1){
		switch(state){
			case STOP_STATE:
				// Remain in the STOP state until we hit the STATE_DELAY (20s release, 5 debug)
				// Transition to stop state from GO state
				// 1. Clear the event flag
				// 2. Call the stop transition
				if(get_timer() >= STATE_DELAY){
					// If we have counted for 20 seconds, start the GO transition
					reset_timer();
					state = GO;
					GO_transition();
				}
				break;
			case GO_STATE:
				// Remain in the GO state until we hit the STATE_DELAY (20s release, 5 debug)
				if(get_timer() >= STATE_DELAY){
					// If we have counted for 20 seconds, start the WARNING transition
					reset_timer();
					state = WARNING;
					WARN_transition();
				}
				break;
			case WARN_STATE:
				// Remain in the warning state until we hit the STATE_DELAY (20s release, 5 debug)
				if(get_timer() >= STATE_DELAY){
					// If we have counted for 20 seconds, start the STOP transition
					reset_timer();	// Reset the time since last reset to account for
					state = WARNING;
					STOP_transition();
				}
				break;
			case CROSS_STATE:

				break;
			default:
				break;
		}
		// Continuously poll the capacitive sensor
	}

}

// Will handle the transition to GO from either stop or cross
void GO_transition(void){
	if(){

	}
	else{
		reset_timer();
	}
	return;
}

void WARN_transition(void){
	reset_timer();
	return;
}

void STOP_transition(void){
	reset_timer();
	return;
}

void cross_handler(void){

//	for(int i = 0; i < 10; i++){
//		continue;
//	}
	return;
}

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
void delay(uint32_t milliseconds)
{
#ifdef DEBUG
	printf("Delay %d [ms]\n", milliseconds);
#endif
	// Multiply milliseconds by ms_ticks to achieve the needed delay
	milliseconds = milliseconds * ms_ticks;
	while((milliseconds--) != 0)
	{
		__asm volatile("nop");
	}
}
