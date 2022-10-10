/*
 * LED.h
 *
 *  Created on: Oct 9, 2022
 *      Author: zande
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

// Specify the PCOR register that we will use for each LED
#define SET_GREEN_LED	GPIOB->PCOR
#define SET_RED_LED		GPIOB->PCOR
#define SET_BLUE_LED	GPIOD->PCOR

// Specify the PSOR register that we will use for each LED
#define CLEAR_GREEN_LED	GPIOB->PSOR
#define CLEAR_RED_LED	GPIOB->PSOR
#define CLEAR_BLUE_LED 	GPIOD->PSOR

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
#define CROSS_Blue		0x30
///////////////////////////////////////////////////////////

typedef struct{
	uint8_t R;
	uint8_t G;
	uint8_t B;
} LED_colors_t;


void LED_init(void);


#endif /* LED_H_ */
