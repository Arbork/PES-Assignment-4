/*
 * @file 	LED.h
 * @brief 	This defines macros, pointers, and functions for use in LED.c
 *
 * @author 	Alexander Bork
 * @date 	Oct 8, 2022
 * @version 1.0
 */

#ifndef LED_H_
#define LED_H_


#include "stdint.h"
#include "stdio.h"
#include "MKL25Z4.h"

// Specify the port that each LED is on
#define LED_GREEN_PORT	PORTB
#define LED_BLUE_PORT	PORTD
#define LED_RED_PORT	PORTB

// Not used for HW #4
//// Specify the PCOR register that we will use for each LED
//#define SET_GREEN_LED	GPIOB->PCOR
//#define SET_RED_LED		GPIOB->PCOR
//#define SET_BLUE_LED	GPIOD->PCOR
//
//// Specify the PSOR register that we will use for each LED
//#define CLEAR_GREEN_LED	GPIOB->PSOR
//#define CLEAR_RED_LED	GPIOB->PSOR
//#define CLEAR_BLUE_LED 	GPIOD->PSOR

// Specify the pin that each LED is on
#define LED_GREEN_PIN	19
#define LED_RED_PIN		18
#define LED_BLUE_PIN	1


///////////////////////////////////////////////////////////
// Colors to use
#define STOP_RED		0x61
#define STOP_GREEN		0x1E
#define STOP_BLUE		0x3C

#define GO_RED			0x22
#define GO_GREEN		0x96
#define GO_BLUE			0x22

#define WARN_RED		0xFF
#define	WARN_GREEN		0xB2
#define WARN_BLUE		0x00

#define CROSS_RED		0x00
#define CROSS_GREEN		0x10
#define CROSS_BLUE		0x30

#define COLOR_OFF		0

// The following defines are the different transitions NO LONGER USED FOR THIS HW
//#define WARN_STOP_DELTA_R	((STOP_RED - WARN_RED) / 16)
//#define WARN_STOP_DELTA_G 	((STOP_GREEN - WARN_GREEN) / 16)
//#define WARN_STOP_DELTA_B	((STOP_BLUE - WARN_BLUE) / 16)
//
//// GO to Warn
//#define GO_WARN_DELTA_R		((WARN_RED - GO_RED) / 16)
//#define GO_WARN_DELTA_G		((WARN_GREEN - GO_GREEN) / 16)
//#define GO_WARN_DELTA_B		((WARN_BLUE - GO_BLUE) / 16)
//
//// Stop to GO
//#define STOP_GO_DELTA_R		((GO_RED - STOP_RED) / 16)
//#define STOP_GO_DELTA_G		((GO_GREEN - STOP_GREEN) / 16)
//#define STOP_GO_DELTA_B		((GO_BLUE - STOP_BLUE) / 16)
//
//// Cross to GO, (negative GO to Cross)
//#define CROSS_GO_DELTA_R	(GO_RED / 16)
//#define CROSS_GO_DELTA_G	((GO_GREEN - CROSS_GREEN) / 16)
//#define CROSS_GO_DELTA_B	((GO_BLUE - CROSS_BLUE) / 16)
//
//// Warn to CROSS
//#define	WARN_CROSS_DELTA_R	((CROSS_RED - WARN_RED) / 16)
//#define	WARN_CROSS_DELTA_G	((CROSS_GREEN - WARN_GREEN) / 16)
//#define WARN_CROSS_DELTA_B	((CROSS_BLUE - WARN_BLUE) / 16)
//
//// Cross to
//#define STOP_CROSS_DELTA_R	((CROSS_RED - STOP_RED) / 16)
//#define STOP_CROSS_DELTA_G	((CROSS_GREEN - STOP_GREEN) / 16)
//#define STOP_CROSS_DELTA_B	((CROSS_BLUE - STOP_BLUE) / 16)
///////////////////////////////////////////////////////////
// TPM Defines
#define	TPM_PERIOD		0xff

// 8 bit values for each color
typedef struct{
	uint8_t R;
	uint8_t G;
	uint8_t B;
} LED_colors_t;

/*
 * @brief	Initialize the GPIO ports, pins, and clocks
 *
 * 		This function initializes all of the functionality required for the GPIO pins that the
 * 	LEDs are on. It enables the clock for the ports that the LEDs are on, clears the PCR registers
 * 	for each LEDs ports, sets the PCR mux for each LED.
 *
 * 	@params	None
 * 	@return	None
 */
void LED_init(void);

/*
 * @brief	Alter the PWM duty cycle of the LEDs
 *
 * 		Whenever a new CnV value is set, the TPM counter is reset. So all 3 LEDs will be driven synchronously
 * 		with the duty cycle of the input value/255
 *
 * @params	Red, Green, Blue	- 8 bit values for the count value to be used for each colors PWM
 * @return 	void
 */
void set_LED_PWM(uint8_t Red, uint8_t Green, uint8_t Blue);

/*
 * @brief Initialize the TPM for the LEDs
 *
 *		Configure TPM0 and 2 to be used in PWM control of the three LEDs.
 *		Use a 48MHz input clock, set a period of 255 with an initial count value of 0.
 *		At the end, initialize the colors to be the STOP colors.
 *
 * @params	None
 * @return	void
 */
void TPM_init(void);

#endif /* LED_H_ */
