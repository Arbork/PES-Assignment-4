/*
 * @file 	touch.c
 * @brief 	Implement functions to setup and read touchpad
 *
 * @author 	Alexander Bork
 * @date 	Oct 8, 2022
 * @version 1.0
 */

#include "touch.h"

// An initial value from the sensor for when nothing is touching it
static uint32_t offset = 0;

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
void touch_setup(void){
	// Enable the clock to the TSI peripheral
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;

	// Clear the GENCS
	TSI0->GENCS = 	TSI_GENCS_MODE(0u) | //operating in non-noise mode
					TSI_GENCS_REFCHRG(0u) | //reference oscillator charge and discharge value 500nA
					TSI_GENCS_DVOLT(0u) | //oscillator voltage rails set to default
					TSI_GENCS_EXTCHRG(0u) | //electrode oscillator charge and discharge value 500nA
					TSI_GENCS_PS(0u) |  // frequency clcok divided by one
					TSI_GENCS_NSCN(31u) | //scanning the electrode 32 times
					TSI_GENCS_TSIEN_MASK | //enabling the TSI module
					TSI_GENCS_EOSF_MASK; // writing one to clear the end of scan flag

	offset = (uint32_t)scan_touchpad();

	return;
}

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
 *	@return data
 *
 */
uint16_t scan_touchpad(void){
	uint16_t data = 0;
	// Measure channel 10
	TSI0->DATA = TSI_DATA_TSICH(CHANNEL);
	// Trigger a software start to the scan
	TSI0->DATA |= TSI_DATA_SWTS_MASK;

	// A busy loop that waits for the scan to complete 32 times
	while(!(TSI0->GENCS & TSI_GENCS_EOSF_MASK));


	// Bit mask the data (lower 16 bits) from the data register
	data = (TSI0->DATA & 0xFFFF);
	// Clear the EOSF
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK;
	return (data - offset);
}





