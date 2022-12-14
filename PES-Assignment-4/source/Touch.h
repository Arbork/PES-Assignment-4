/*
 * @file 	Touch.h
 * @brief 	Macros used for touch-pad functionality and functions
 *
 * @author 	Alexander Bork
 * @date 	Oct 8, 2022
 * @version 1.0
 */

#ifndef TOUCH_H_
#define TOUCH_H_


#include "MKL25Z4.h"
#include "stdint.h"

// Macros for initializing the touchpad
#define CHANNEL			10u
#define TSI_NSCN		TSI_GENCS_NSCN(0b11111)
#define TSI_TSIEN		TSI_GENCS_TSIEN_MASK
#define TSI_TSIIEN		TSI_GENCS_TSIIEN_MASK(0b0)
#define TSI_EOSF		TSI_GENCS_EOSF_MASK
#define NOISE_TOLERANCE	20


/*
 * 	@brief Set up the touchpad for use
 *
 * 		This function sets up the touch sensor for use in this project.
 * 	I enable the clock to the TSI periphal, clear the GENCS register for TSI0, clear the scan flag,
 * 	and specify that any request to scan performs 32 electrode scans.
 *
 * 	THIS CODE WAS PULLED AND ADAPTED FROM ALEXANDER G. DEAN'S GITHUB
 *		https://github.com/alexander-g-dean/ESF/blob/master/NXP/Misc/Touch%20Sense/TSI/src/main.c
 *
 *	@params	None
 *	@return void
 */
void touch_setup();



/*
 * @brief Read the value from the touchpad
 *
 * 		This function scans the touch sensor peripheral. It specifies that we want to scan
 * 	channel 10 of the touch sensor, which we trigger via software, and we wait for it to perform
 * 	32 electrode scans. It will then return an integer based on its scan, and we return that integer
 * 	minus an offset for a given sensitivity.
 *
 *	THIS CODE WAS PULLED AND ADAPTED FROM ALEXANDER G. DEAN'S GITHUB
 *		https://github.com/alexander-g-dean/ESF/blob/master/NXP/Misc/Touch%20Sense/TSI/src/main.c
 *
 *	@params None
 *	@return data	16 bit value that was read from the touch sensor
 *
 */
uint16_t scan_touchpad(void);

#endif /* TOUCH_H_ */
