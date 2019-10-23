/*
 * File:	gpio.h
 * Purpose:
 *
 * Notes:
 */

#ifndef __GPIO_H__
#define __GPIO_H__

/******************************************************************************/

// Function prototypes
void gpio_init(void);


#define ZLG7290_INT_PIN    (9)
#define ZLG7290_INT_DIR    GPIOB_PDIR
#define ZLG7290_INT_DDR    GPIOB_PDDR
#define ZLG7290_INT_PCR    PORTB_PCR9

#define ZLG7290_INT()   BIT_IS_HIGH(ZLG7290_INT_DIR,ZLG7290_INT_PIN)

//REED_INT
#define REED_PIN         (6)
#define REED_DIR        GPIOE_PDIR
#define REED_DDR        GPIOE_PDDR
#define REED_PCR        PORTE_PCR6




#define REED_STATUS        BIT_IS_HIGH(REED_DIR , REED_PIN)


#define DIRECTION_A_PIN         (20)
#define DIRECTION_A_DOR        GPIOB_PDOR
#define DIRECTION_A_DDR        GPIOB_PDDR
#define DIRECTION_A_PCR        PORTB_PCR20

#define DIRECTION_B_PIN         (17)
#define DIRECTION_B_DOR        GPIOB_PDOR
#define DIRECTION_B_DDR        GPIOB_PDDR
#define DIRECTION_B_PCR        PORTB_PCR17

#define MOTORA_PWM_POS()         SET_BIT(DIRECTION_A_DOR, DIRECTION_A_PIN)
#define MOTORA_PWM_NEG()         CLR_BIT(DIRECTION_A_DOR, DIRECTION_A_PIN)

#define MOTORB_PWM_POS()         SET_BIT(DIRECTION_B_DOR, DIRECTION_B_PIN)
#define MOTORB_PWM_NEG()         CLR_BIT(DIRECTION_B_DOR, DIRECTION_B_PIN)

//Led
#define LED_0_PIN         (3)
#define LED_0_DOR        GPIOD_PDOR
#define LED_0_DDR        GPIOD_PDDR
#define LED_0_PCR        PORTD_PCR3

#define LED_1_PIN         (6)
#define LED_1_DOR        GPIOD_PDOR
#define LED_1_DDR        GPIOD_PDDR
#define LED_1_PCR        PORTD_PCR4


#define LED_2_PIN         (2)
#define LED_2_DOR        GPIOE_PDOR
#define LED_2_DDR        GPIOE_PDDR
#define LED_2_PCR        PORTE_PCR5

#define LED_0_OFF()  SET_BIT(LED_0_DOR, LED_0_PIN)
#define LED_1_OFF()  SET_BIT(LED_1_DOR, LED_1_PIN)
#define LED_2_OFF()  SET_BIT(LED_2_DOR, LED_2_PIN)

#define LED_0_ON()  CLR_BIT(LED_0_DOR, LED_0_PIN)
#define LED_1_ON()  CLR_BIT(LED_1_DOR, LED_1_PIN)
#define LED_2_ON()  CLR_BIT(LED_2_DOR, LED_2_PIN)

#define LED_0_REV()  REV_BIT(LED_0_DOR, LED_0_PIN)
#define LED_1_REV()  REV_BIT(LED_1_DOR, LED_1_PIN)
#define LED_2_REV()  REV_BIT(LED_2_DOR, LED_2_PIN)


#define LED_BLUE_OFF()    LED_0_OFF()
#define LED_ORANGE_OFF()  LED_1_OFF()
#define LED_GREEN_OFF()   LED_2_OFF()

#define LED_BLUE_ON()     LED_0_ON()
#define LED_ORANGE_ON()   LED_1_ON()
#define LED_GREEN_ON()    LED_2_ON()

/*²¦Âë¿ª¹Ø*/
#define REED0_PIN         (2)
#define REED0_DIR        GPIOD_PDIR
#define REED0_DDR        GPIOD_PDDR
#define REED0_PCR        PORTD_PCR2

#define REED1_PIN         (5)
#define REED1_DIR        GPIOD_PDIR
#define REED1_DDR        GPIOD_PDDR
#define REED1_PCR        PORTD_PCR5

#define REED2_PIN         (7)
#define REED2_DIR        GPIOD_PDIR
#define REED2_DDR        GPIOD_PDDR
#define REED2_PCR        PORTD_PCR7

#define REED3_PIN         (3)
#define REED3_DIR        GPIOE_PDIR
#define REED3_DDR        GPIOE_PDDR
#define REED3_PCR        PORTE_PCR3

#define REED0_STATUS        BIT_IS_HIGH(REED0_DIR , REED0_PIN)
#define REED1_STATUS        BIT_IS_HIGH(REED1_DIR , REED1_PIN)
#define REED2_STATUS        BIT_IS_HIGH(REED2_DIR , REED2_PIN)
#define REED3_STATUS        BIT_IS_HIGH(REED3_DIR , REED3_PIN)

/******************************************************************************/
#endif /* __GPIO_H__ */








