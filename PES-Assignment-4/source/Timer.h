/*
 * @file 	Timer.h
 * @brief 	Defines fxn prototypes for timer functionality
 *
 * @author 	Alexander Bork
 * @date 	Oct 8, 2022
 * @version 1.0
 */

#ifndef TIMER_H_
#define TIMER_H_


#include "MKL25Z4.h"
#include "stdio.h"
#include "stdint.h"
#include "core_cm0plus.h"

typedef uint32_t 	ticktime_t;

#define INPUT_FREQ		48000000UL
#define OUTPUT_FREQ		16
#define	SYS_COUNTER		(INPUT_FREQ / (16 * OUTPUT_FREQ))



// Function definitions
/*
 * @brief	Initializing the SysTick counter
 *
 * 		Sets the counter value and irq priority then enables Systick
 *
 * @params	None
 * @return	void
 */
void init_systick(void);

/*
 * @brief	Returns the time since startup
 *
 * @params 	None
 * @return	timeSinceBoot	- The time since bootup
 */
ticktime_t now(void);

/*
 * @brief	Reset the tiemr
 *
 * 		Sets TSR to be TSB so that get_timer returns 0
 * @params	None
 * @return	void
 */
void reset_timer(void);

/*
 * @brief	Get the difference btwn TSB and TSR
 *
 * @params	None
 * @return	The difference in time between TSB and TSR
 */
ticktime_t get_timer(void);


/*
 * @brief The interrupt handler for Systick
 *
 * 		Increments time since boot every interrupt
 *
 * @params	None
 * @return	void
 */
void SysTick_Handler(void);


#endif /* TIMER_H_ */
