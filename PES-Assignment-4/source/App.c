/*
 * @file 	app.c
 * @brief 	Group of functions that drive the project's functionality
 *
 * @author 	Alexander Bork
 * @date 	October 8th, 2022
 * @version 1.0
 */


#include "App.h"

static LED_colors_t currColors;
static lightState_t prevState;
static lightState_t state;

const char * state_list[] = {"CROSS", "STOP", "GO", "WARNING"};


#ifdef DEBUG

/*
 * @brief	Logs any state change
 *
 *		Use the previous state and state variables to print the state change over UART with a timestamp
 *
 * @params	None
 * @return	void
 */
void log_state_change(void){
	logger("From State: %s -> %s @ t=%d\r\n", state_list[prevState], state_list[state], (now() * SYSTICK_MS_CONVERSION));
}

/*
 * @brief	Logs any button press
 *
 *		Print the timestamp of the Crosswalk request via UART.
 *
 * @params	timestamp	- An integer signifying the number of 62.5ms periods we have passed
 * @return	void
 */
void log_button_press(ticktime_t timestamp){
	logger("Crosswalk requested at t=%d\r\n", (timestamp * SYSTICK_MS_CONVERSION));
}
#endif

/**
 * @brief	Calls other initialization functions in a packaged manner
 *
 * 	The app setup function makes calls to the gpio setup and the touch setup to initialize
 *	the clock to the LEDs, sets multiplexing of the pins used for our 3 LEDs, send the clock to
 *	the Touch Slider, and initialize it as well.
 *
 * @params	None
 * @return 	void
 */
void app_init(void){
	prevState = STOP_STATE;
	state = STOP_STATE;
	currColors.R = STOP_RED;
	currColors.G = STOP_GREEN;
	currColors.B = STOP_BLUE;
	LED_init();
	TPM_init();
	touch_setup();
}

/*
 *	@brief	Runs the state machine indefinitely
 *
 *		This function handles the state machine of the project. It continuously polls the capacitive sensor
 *		at a period of 62.5 ms to determine if a transition to the crosswalk state is needed.
 *		Each case in the switch statement handles delays for the amount of time each state needs to last.
 *
 *	@params	None
 *	@return	void/doesn't return
 */
void app_loop(void){
	ticktime_t elapsed_time = 0;
	uint16_t scan = 0;
	init_systick();
	while(1){
		switch(state){
			case STOP_STATE:
				// Remain in the STOP state until we hit the STATE_DELAY (20s release, 5s debug)
				if(elapsed_time >= STATE_DELAY){
					// Transition to the next state
					prevState = STOP_STATE;
					state = GO_STATE;
					transition(GO_RED, GO_GREEN, GO_BLUE);
				#ifdef DEBUG
					log_state_change();
				#endif
					elapsed_time = 0;
				}
				break;
			case GO_STATE:
				// Remain in the GO state until we hit the STATE_DELAY (20s release, 5s debug)
				if(elapsed_time >= STATE_DELAY){
					// Transition to the WARNING state
					prevState = GO_STATE;
					state = WARN_STATE;
					transition(WARN_RED, WARN_GREEN, WARN_BLUE);
				#ifdef DEBUG
					log_state_change();
				#endif
					elapsed_time = 0;
				}
				break;
			case WARN_STATE:
				// Remain in the warning state until we hit the STATE_DELAY (20s release, 5s debug)
				if(elapsed_time >= WARN_DELAY){
					// Transition to the STOP state
					prevState = WARN_STATE;
					state = STOP_STATE;
					transition(STOP_RED, STOP_GREEN, STOP_BLUE);
				#ifdef DEBUG
					log_state_change();
				#endif
					elapsed_time = 0;
				}
				break;
			case CROSS_STATE:
				elapsed_time = 0;
				// We wait for 10s to let the cross walk run. Loop executes 10 times
				while(elapsed_time < CROSS_DELAY){
					// Turn off and then wait for 250ms
					set_LED_PWM(COLOR_OFF, COLOR_OFF, COLOR_OFF);
					delay(CROSS_OFF_TICKS);
					// Turn on LEDs and wait for 750ms
					set_LED_PWM(CROSS_RED, CROSS_GREEN, CROSS_BLUE);
					delay(CROSS_ON_TICKS);
					elapsed_time += STATE_TRANSITION;
				}
				prevState = CROSS_STATE;
				state = GO_STATE;
			#ifdef DEBUG
				log_state_change();
			#endif
				// Transition back to Green
				transition(GO_RED, GO_GREEN, GO_BLUE);
				elapsed_time = 0;
				break;
			default:
				break;
		}
		delay(DELAY_625MS);
		elapsed_time++;
		// Scan the touchpad to see if it has been touched
		scan = (uint16_t)scan_touchpad() + NOISE_TOLERANCE;
		// Continuously poll the capacitive sensor
		if((scan > TOUCH_NOISE_THRESHOLD) && (state != CROSS_STATE)){
		#ifdef DEBUG
			log_button_press(now());
		#endif
			prevState = state;	// Update the previous state before we alter the current state
			state = CROSS_STATE;
#ifdef DEBUG
	log_state_change();
#endif
			transition(CROSS_RED, CROSS_GREEN, CROSS_BLUE);
			elapsed_time = 0;	// Reset timer and scan values so the next iteration has to retrieve new values
			scan = 0;
		}
	}

}

/*
 *	@brief	Handles the transition of colors during state changes
 *
 *		This function handles the change of colors between states to ensure there are no headaches.
 *		For 16 periods of 62.5ms (1s), we will update the colors being lit by a small increment.
 *		If the capacitive sensor is ever pressed, then we will restart the process but have the crosswalk colors
 *		as the target.
 *
 *	@params	final_red, final_green, final_blue	- Integers signifying the final value to be put in the TPM ctr register for each color
 *	@return	void/doesn't return
 */
void transition(uint8_t final_red, uint8_t final_green, uint8_t final_blue){
	uint8_t init_R = currColors.R;
	uint8_t init_G = currColors.G;
	uint8_t init_B = currColors.B;
	uint16_t scan = 0;
	uint32_t delta_T = 0;		// This is the elapsed time since we started the color transition
	while(delta_T < STATE_TRANSITION){
		// Delay 62.5 ms before reading cap sensor
		delay(DELAY_625MS);
		scan = (uint16_t)scan_touchpad() + NOISE_TOLERANCE;
		if((scan > TOUCH_NOISE_THRESHOLD) && (state != CROSS_STATE)){
			// Alter the target colors if the cap sensor is pressed
			final_red = CROSS_RED;
			final_green = CROSS_GREEN;
			final_blue = CROSS_BLUE;
			init_R = currColors.R;
			init_G = currColors.G;
			init_B = currColors.B;
			// Reset the timer so we transition colors over a 1s period
			delta_T = 0;
			state = CROSS_STATE;

		}
		// Update each color with a delta and display the change
		currColors.R += ((final_red - init_R) / 16);
		currColors.G += ((final_green - init_G) / 16);
		currColors.B += ((final_blue - init_B) / 16);
		set_LED_PWM(currColors.R, currColors.G, currColors.B);
		delta_T++;

	}
	currColors.R = final_red;
	currColors.B = final_blue;
	currColors.G = final_green;
	// Set color to target colors in case there is drift in the arithmetic for changing the color
	set_LED_PWM(final_red, final_green, final_blue);
	return;
}


/*
 * @brief	This function delays the program without being optimized out
 *
 * 		This function creates a delay in # of systick interrupts as specified by the
 * 		input parameter. Each systick corresponds to a delay of 62.5ms
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
