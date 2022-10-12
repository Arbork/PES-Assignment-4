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
	LED_BLUE_PORT->PCR[LED_BLUE_PIN] |= PORT_PCR_MUX(4);
	LED_GREEN_PORT->PCR[LED_GREEN_PIN] |= PORT_PCR_MUX(3);
	LED_RED_PORT->PCR[LED_RED_PIN] |= PORT_PCR_MUX(3);

	// Set each LED to an output
//	GPIOB->PDDR |= (1 << LED_RED_PIN);
//	GPIOB->PDDR |= (1 << LED_GREEN_PIN);
//	GPIOD->PDDR |= (1 << LED_BLUE_PIN);

	return;
}

void TPM_init(void){
	// Configure both TPM0 and 2
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	// Set clock source for TPM to be 48MHz
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);
	// Set period for TPM0 and TPM2
	TPM0->MOD = TPM_PERIOD;
	TPM2->MOD = TPM_PERIOD;
	// Count up with prescaler of 2
	TPM0->SC = TPM_SC_PS(1);
	TPM2->SC = TPM_SC_PS(1);
	// Operate in Debug Mode
	TPM0->CONF |= TPM_CONF_DBGMODE(3);
	TPM2->CONF |= TPM_CONF_DBGMODE(3);
	// Set channels to edge aligned low-true PWM
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;	// Blue channel
	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;	// Green channel
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;	// Red channel
	// Initialize duty cycle
	TPM0->CONTROLS[1].CnV = 0;	// Blue channel value
	TPM2->CONTROLS[1].CnV = 0;	// Green channel value
	TPM2->CONTROLS[0].CnV = 0; 	// Red channel value
	// Start the TPM
	TPM0->SC |= TPM_SC_CMOD(1);
	TPM2->SC |= TPM_SC_CMOD(1);
	// NEED TO INIT COLORS TO RED
	set_LED_PWM((uint8_t)STOP_RED, (uint8_t)STOP_GREEN, (uint8_t)STOP_BLUE);
}

/*
 * EDIT AND VERIFY THIS
 * Make sure that it is a critical section so that all three duty cycles are synchronized
 */
void set_LED_PWM(uint8_t Red, uint8_t Green, uint8_t Blue){
	TPM2->CONTROLS[0].CnV = Red;	// Sets duty cycle to be (Red/0xff)
	TPM2->CONTROLS[1].CnV = Green;	// Sets duty cycle to be (Green/0xff)
	TPM0->CONTROLS[1].CnV = Blue;	// Sets duty cycle to be (Blue/0xff) for blue led
}

