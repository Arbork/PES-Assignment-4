/*
 * @file 	app.h
 * @brief 	Macros and fxns for project functionality
 *
 * @author 	Alexander Bork
 * @date 	Oct 8th, 2022
 * @version 1.0
 */

#ifndef APP_H_
#define APP_H_

#include "LED.h"
#include "Touch.h"
#include "Timer.h"
#include "logger.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"


#define STATE_TRANSITION		16
#define DELAY_625MS				1		// The number of periods to produce a 62.5ms delay
#define TOUCH_NOISE_THRESHOLD	50
#define CROSS_DELAY				160		// # of periods for entire CW state
#define CROSS_OFF_TICKS			4		// # of 62.5ms periods to wait when CW is off
#define CROSS_ON_TICKS			12		// # of 62.5ms periods to wait when CW is on


#ifndef DEBUG
	#define STATE_DELAY		320
	#define WARN_DELAY		80
#else
	#define	STATE_DELAY 	80
	#define WARN_DELAY		48
	#define SYSTICK_MS_CONVERSION	63
#endif

// Enumeration for all of the possible states
typedef enum{
	CROSS_STATE,
	STOP_STATE,
	GO_STATE,
	WARN_STATE
} lightState_t;



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
void app_init(void);

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
void app_loop(void);


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
void delay(uint32_t systicks);

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
void transition(uint8_t final_red, uint8_t final_green, uint8_t final_blue);


#ifdef DEBUG
/*
 * @brief	Logs any state change
 *
 *		Use the previous state and state variables to print the state change over UART with a timestamp
 *
 * @params	None
 * @return	void
 */
void log_state_change(void);

/*
 * @brief	Logs any button press
 *
 *		Print the timestamp of the Crosswalk request via UART.
 *
 * @params	timestamp	- An integer signifying the number of 62.5ms periods we have passed
 * @return	void
 */
void log_button_press(ticktime_t timestamp);
#endif


#endif /* APP_H_ */
