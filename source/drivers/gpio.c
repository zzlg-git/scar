/*
* File:	start.c
* Purpose: gpio initialization
*
* Notes:
*/
#include "common.h"
#include "gpio.h"

/******************************************************************************/
/*!
* \brief   gpio initialization
* \return  None
*
* This function initializes the ports used as gpio
*/
void gpio_init(void)
{

  ZLG7290_INT_PCR = (0 | PORT_PCR_MUX(1));

  REED_PCR = (0 | PORT_PCR_MUX(1));
  
  
  REED0_PCR = (0 | PORT_PCR_MUX(1));
  REED1_PCR = (0 | PORT_PCR_MUX(1));
  REED2_PCR = (0 | PORT_PCR_MUX(1));
  REED3_PCR = (0 | PORT_PCR_MUX(1));

  DIRECTION_A_PCR = (0 | PORT_PCR_MUX(1));
  DIRECTION_B_PCR = (0 | PORT_PCR_MUX(1));

  LED_0_PCR = (0 | PORT_PCR_MUX(1));
  LED_1_PCR = (0 | PORT_PCR_MUX(1));
  LED_2_PCR = (0 | PORT_PCR_MUX(1));

  SET_BIT(LED_0_DDR, LED_0_PIN);
  SET_BIT(LED_1_DDR, LED_1_PIN);
  SET_BIT(LED_2_DDR, LED_2_PIN);

  SET_BIT(LED_0_DOR, LED_0_PIN);
  SET_BIT(LED_1_DOR, LED_1_PIN);
  SET_BIT(LED_2_DOR, LED_2_PIN);

  CLR_BIT(ZLG7290_INT_DDR, ZLG7290_INT_PIN);
  CLR_BIT(REED_DDR, REED_PIN);
  CLR_BIT(REED0_DDR, REED0_PIN);
  CLR_BIT(REED1_DDR, REED1_PIN);
  CLR_BIT(REED2_DDR, REED2_PIN);
  CLR_BIT(REED3_DDR, REED3_PIN);
  SET_BIT(DIRECTION_A_DDR, DIRECTION_A_PIN);
  SET_BIT(DIRECTION_B_DDR, DIRECTION_B_PIN);
  
  PORTC_PCR8 = (0|PORT_PCR_MUX(1)|PORT_PCR_PE_MASK);
  CLR_BIT(GPIOC_PDDR,8);
  PORTC_PCR9 = (0|PORT_PCR_MUX(1)|PORT_PCR_PE_MASK);
  CLR_BIT(GPIOC_PDDR,9);
  PORTC_PCR10 = (0|PORT_PCR_MUX(1)|PORT_PCR_PE_MASK);
  CLR_BIT(GPIOC_PDDR,10);
  PORTC_PCR11 = (0|PORT_PCR_MUX(1)|PORT_PCR_PE_MASK);
  CLR_BIT(GPIOC_PDDR,11);
  PORTC_PCR12 = (0|PORT_PCR_MUX(1)|PORT_PCR_PE_MASK);
  CLR_BIT(GPIOC_PDDR,12);
  PORTC_PCR13 = (0|PORT_PCR_MUX(1)|PORT_PCR_PE_MASK);
  CLR_BIT(GPIOC_PDDR,13);
  PORTC_PCR14 = (0|PORT_PCR_MUX(1)|PORT_PCR_PE_MASK);
  CLR_BIT(GPIOC_PDDR,14);
  PORTC_PCR15 = (0|PORT_PCR_MUX(1)|PORT_PCR_PE_MASK);
  CLR_BIT(GPIOC_PDDR,15);
} // gpio_init

/******************************************************************************/



