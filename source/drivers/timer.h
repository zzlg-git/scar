/*
 * File:	timer.h
 * Purpose:
 *
 * Notes:
 */

#ifndef __TIMER_H__
#define __TIMER_H__

/******************************************************************************/

// Function prototypes
void timer_init(void);

// define, interface
/* To abort the current cycle and start a timer period with the new value,
 * the timer must be disabled and enabled again.
 */
#define TIME0_RESET()       {PIT_TCTRL0=0x00;PIT_TCTRL0=0x03;}
#define TIME0_GET           (PIT_CVAL0>>5) // depend on the bus frequency

#define TIME1_RESET()       {PIT_TCTRL1=0x00;PIT_TCTRL1=0x01;}
#define TIME1_GET_uS        ((0xFFFFFFFFUL-PIT_CVAL1)/periph_clk_mhz)

#define TIME2_RESET()       {PIT_TCTRL2=0x00;PIT_TCTRL2=0x01;}
#define TIME2_GET_uS        ((0xFFFFFFFFUL-PIT_CVAL2)/periph_clk_mhz)

/*
 * This timer is used by Delay_T3_uS(), Do not use it for other purposes !!
 */
#define TIME3_RESET()       {PIT_TCTRL3=0x00;PIT_TCTRL3=0x01;}
#define TIME3_GET_uS()      ((0xFFFFFFFFUL-PIT_CVAL3)/periph_clk_mhz)

/******************************************************************************/
#endif /* __TIMER_H__ */
