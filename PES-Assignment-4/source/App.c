/*
 * App.c
 *
 *  Created on: Oct 9, 2022
 *      Author: zande
 */


#include "App.h"

static LED_colors_t currColors;
static LED_colors_t nextColors;
static lightState_t state;
static uint8_t 		cross;

void app_init(void){
	cross = false;
	state = STOP_STATE;
	currColors.R = STOP_RED;
	currColors.G = STOP_GREEN;
	currColors.B = STOP_BLUE;
	nextColors.R = STOP_RED;
	nextColors.G = STOP_GREEN;
	nextColors.B = STOP_BLUE;
	LED_init();
	TPM_init();
	touch_setup();
}

void app_loop(void){
	ticktime_t elapsed_time = 0;
	uint16_t scan = 0;
	init_systick();
	while(1){
		switch(state){
			case STOP_STATE:
				// Remain in the STOP state until we hit the STATE_DELAY (20s release, 5 debug)
				if(elapsed_time >= STATE_DELAY){
					// Transition to the next state
					state = GO_STATE;
					transition(GO_RED, GO_GREEN, GO_BLUE);
					elapsed_time = 0;
				}
				break;
			case GO_STATE:
				// Remain in the GO state until we hit the STATE_DELAY (20s release, 5 debug)
				if(elapsed_time >= STATE_DELAY){
					state = WARN_STATE;
					transition(WARN_RED, WARN_GREEN, WARN_BLUE);
					elapsed_time = 0;
				}
				break;
			case WARN_STATE:
				// Remain in the warning state until we hit the STATE_DELAY (20s release, 5 debug)
				if(elapsed_time >= WARN_DELAY){
					// If we have counted for 20 seconds, start the STOP transition
					state = STOP_STATE;
					transition(STOP_RED, STOP_GREEN, STOP_BLUE);
					elapsed_time = 0;
				}
				break;
			case CROSS_STATE:
				elapsed_time = 0;
				// We wait for 10s to let the cross walk run
				while(elapsed_time < CROSS_DELAY){
					// Turn off and then wait for 250ms
					set_LED_PWM(COLOR_OFF, COLOR_OFF, COLOR_OFF);
					delay(CROSS_OFF_TICKS);
					// Turn on LEDs and wait for 750ms
					set_LED_PWM(CROSS_RED, CROSS_GREEN, CROSS_BLUE);
					delay(CROSS_ON_TICKS);
					elapsed_time += STATE_TRANSITION;
				}
				state = GO_STATE;
				transition(GO_RED, GO_GREEN, GO_BLUE);
				elapsed_time = 0;
				break;
			default:
				break;
		}
		delay(DELAY_65MS);
		elapsed_time++;
		// Scan the touchpad to see if it has been touched
		scan = (uint16_t)scan_touchpad() + NOISE_TOLERANCE;
		// Continuously poll the capacitive sensor
		if((scan > TOUCH_NOISE_THRESHOLD) && (state != CROSS_STATE)){
			transition(CROSS_RED, CROSS_GREEN, CROSS_BLUE);
			state = CROSS_STATE;
			elapsed_time = 0;
			scan = 0;
		}
	}

}

void transition(uint8_t final_red, uint8_t final_green, uint8_t final_blue){
	uint8_t init_R = currColors.R;
	uint8_t init_G = currColors.G;
	uint8_t init_B = currColors.B;
	uint16_t scan = 0;
	uint32_t delta_T = 0;		// This is the elapsed time since we started the color transition
	while(delta_T < STATE_TRANSITION){
		delay(DELAY_65MS);
		scan = (uint16_t)scan_touchpad() + NOISE_TOLERANCE;
		if((scan > TOUCH_NOISE_THRESHOLD) && (state != CROSS_STATE)){
			final_red = CROSS_RED;
			final_green = CROSS_GREEN;
			final_blue = CROSS_BLUE;
			init_R = currColors.R;
			init_G = currColors.G;
			init_B = currColors.B;
			delta_T = 0;
			state = CROSS_STATE;
		}
		currColors.R += ((final_red - init_R) / 16);
		currColors.G += ((final_green - init_G) / 16);
		currColors.B += ((final_blue - init_B) / 16);
		set_LED_PWM(currColors.R, currColors.G, currColors.B);
		delta_T++;

	}
	currColors.R = final_red;
	currColors.B = final_blue;
	currColors.G = final_green;
	// Set color to target in case there is drift in the arithmetic for changing the color
	set_LED_PWM(final_red, final_green, final_blue);
	return;
}

//void transition(uint8_t end_state){
//	uint8_t init_R = colors.R;
//	uint8_t init_G = colors.G;
//	uint8_t init_B = colors.B;
//	uint32_t threshold = 0;		// Threshold ensures that we only update the PWM of each LED every (1/16)ms
//	uint32_t delta_T = 0;		// This is the elapsed time since we started the color transition
//	reset_timer();
//	while(delta_T < STATE_TRANSITION){
//		delta_T = get_timer();
//		switch(end_state){
//			case CROSS_STATE:
//				if((state == STOP_STATE) && (threshold <= delta_T)){
//					init_R += STOP_CROSS_DELTA_R;
//					init_G += STOP_CROSS_DELTA_G;
//					init_B += STOP_CROSS_DELTA_B;
//					set_LED_PWM(init_R, init_G, init_B);
//					threshold++;
//				}
//				else if((state == GO_STATE) && (threshold <= delta_T)){
//					init_R -= CROSS_GO_DELTA_R;
//					init_G -= CROSS_GO_DELTA_G;
//					init_B -= CROSS_GO_DELTA_B;
//					set_LED_PWM(init_R, init_G, init_B);
//					threshold++;
//				}
//				else if((state == WARN_STATE) && (threshold <= delta_T)){
//					init_R += WARN_CROSS_DELTA_R;
//					init_G += WARN_CROSS_DELTA_G;
//					init_B += WARN_CROSS_DELTA_B;
//					set_LED_PWM(init_R, init_G, init_B);
//					threshold++;
//				}
//				break;
//			case GO_STATE:
//				// Wait for the next SysTick irpt AND check the previous state
//				if((state == CROSS_STATE) && (threshold <= delta_T)){
//					init_R += CROSS_GO_DELTA_R;
//					init_G += CROSS_GO_DELTA_G;
//					init_B += CROSS_GO_DELTA_B;
//					set_LED_PWM(init_R, init_G, init_B);
//					threshold++;
//				}
//				else if((state == STOP_STATE) && (threshold <= delta_T)){
//					init_R += STOP_GO_DELTA_R;
//					init_G += STOP_GO_DELTA_G;
//					init_B += STOP_GO_DELTA_B;
//					set_LED_PWM(init_R, init_G, init_B);
//					threshold++;
//				}
//				break;
//			case STOP_STATE:
//				// Wait for the next SysTick irpt
//				if(threshold <= delta_T){
//					init_R += WARN_STOP_DELTA_R;
//					init_G += WARN_STOP_DELTA_G;
//					init_B += WARN_STOP_DELTA_B;
//					set_LED_PWM(init_R, init_G, init_B);
//					threshold++;
//				}
//				break;
//			case WARN_STATE:
//				// Wait for the next SysTick irpt
//				if(threshold <= delta_T){
//					init_R += GO_WARN_DELTA_R;
//					init_G += GO_WARN_DELTA_G;
//					init_B += GO_WARN_DELTA_B;
//					set_LED_PWM(init_R, init_G, init_B);
//					threshold++;
//				}
//				break;
//		}
//	}
//	colors.R = init_R;
//	colors.G = init_G;
//	colors.B = init_B;
//	state = end_state;
//	// This accounts for any drift that may have occurred since we have a transition of 1/16 each cycle
//	switch(end_state){
//		case CROSS_STATE:
//			set_LED_PWM(CROSS_RED, CROSS_GREEN, CROSS_BLUE);
//			break;
//		case GO_STATE:
//			set_LED_PWM(GO_RED, GO_GREEN, GO_BLUE);
//			break;
//		case STOP_STATE:
//			set_LED_PWM(STOP_RED, STOP_GREEN, STOP_BLUE);
//			break;
//		case WARN_STATE:
//			set_LED_PWM(WARN_RED, WARN_GREEN, WARN_BLUE);
//			break;
//	}
//	reset_timer();
//	return;
//}
//


/*
 * @brief	This function delays the program without being optimized out
 *
 * 		This function creates a delay in # of systick interrupts as specified by the
 * 		input parameter.
 *
 *
 * @param 	systicks	The number of SysTick interrupts to delay for
 * @return	void
 */
void delay(uint32_t systicks)
{
	reset_timer();
	// Multiply milliseconds by ms_ticks to achieve the needed delay
	while(get_timer() < systicks)
	{
		__asm volatile("nop");
	}
}
