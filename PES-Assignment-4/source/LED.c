/*
 * LED.c
 *
 *  Created on: Oct 9, 2022
 *      Author: zande
 */


#include "LED.h"

// Do I need?
//static LED_color_t colors;

/*
 * @brief	Initialize the GPIO ports, pins, and clocks
 *
 * 		This function initializes all of the functionality required for the GPIO pins that the
 * 	LEDs are on. It enables the clock for the ports that the LEDs are on, clears the PCR registers
 * 	for each LEDs ports, sets the PCR mux for each LED, and sets each LED to be an output.
 *
 * 	@params	None
 * 	@return	None
 */
void LED_init(void){

	// Enable the clock for the necessary ports LEDs
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

	// Clear the PCR register for green, blue, and red LED's ports
	LED_GREEN_PORT->PCR[LED_GREEN_PIN] &= ~PORT_PCR_MUX_MASK;
	LED_RED_PORT->PCR[LED_RED_PIN] &= ~PORT_PCR_MUX_MASK;
	LED_BLUE_PORT->PCR[LED_BLUE_PIN] &= ~PORT_PCR_MUX_MASK;

	// Set the PCR mux for each LED's pin
	LED_BLUE_PORT->PCR[LED_BLUE_PIN] |= PORT_PCR_MUX(1);
	LED_GREEN_PORT->PCR[LED_GREEN_PIN] |= PORT_PCR_MUX(1);
	LED_RED_PORT->PCR[LED_RED_PIN] |= PORT_PCR_MUX(1);

	// Set each LED to an output
	GPIOB->PDDR |= (1 << LED_RED_PIN);
	GPIOB->PDDR |= (1 << LED_GREEN_PIN);
	GPIOD->PDDR |= (1 << LED_BLUE_PIN);

	/*
	 * NEED TO INIT COLORS TO RED
	 */
	set_LED_PWM(STOP_RED, STOP_GREEN, STOP_BLUE);
	return;
}

/*
 * EDIT AND VERIFY THIS
 * Make sure that it is a critical section
 */
void set_LED_PWM(uint8_t Red, uint8_t Green, uint8_t Blue){
	uint32_t masking_state = __get_PRIMASK();
	__disable_irq();
	TPM2->CONTROLS[0].CnV = Red;
	TPM2->CONTROLS[1].CnV = Green;
	TPM0->CONTROLS[1].CnV = Blue;
	__set_PRIMASK(masking_state);
}

