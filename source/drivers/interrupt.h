/*
 * File:		interrupt.h
 * Purpose:
 *
 * Notes:
 */

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__
/******************************************************************************/

/* ARM Cortex M4 implementation for interrupt priority shift */
#define ARM_INTERRUPT_LEVEL_BITS    4


#define INTERRUPT_VSYNC_PIN       (6)                  //IRQ
#define INTERRUPT_VSYNC_DDR       GPIOC_PDDR
#define INTERRUPT_VSYNC_PCR       PORTC_PCR6
#define INTERRUPT_VSYNC_IRQ       (89) // PTc

#define INTERRUPT_HREF_PIN      (0)//(5)                // O/E        
#define INTERRUPT_HREF_DDR      GPIOD_PDDR//GPIOD_PDDR
#define INTERRUPT_HREF_PCR      PORTD_PCR0//PORTD_PCR5
#define INTERRUPT_HREF_IRQ      (90)//(62) // PTD

/* Set Interrupt Vector Table Function Pointers */
#undef VECTOR_106 // INTERRUPT_VSYNC_IRQ + 16
#define VECTOR_106    href_isr

#undef VECTOR_105 // INTERRUPT_HREF_IRQ +16
#define VECTOR_105   vsync_isr

#define DISABLE_INTERRUPT_HREF()     disable_irq(INTERRUPT_HREF_IRQ)
#define ENABLE_INTERRUPT_HREF()      enable_irq(INTERRUPT_HREF_IRQ)

#define DISABLE_INTERRUPT_VSYNC()    disable_irq(INTERRUPT_VSYNC_IRQ) 
#define ENABLE_INTERRUPT_VSYNC()     enable_irq(INTERRUPT_VSYNC_IRQ)

#define CLR_FLAG_HREF()      INTERRUPT_HREF_PCR  |= PORT_PCR_ISF_MASK
#define CLR_FLAG_RISE()      INTERRUPT_RISE_PCR |= PORT_PCR_ISF_MASK
#define CLR_FLAG_VSYNC()     INTERRUPT_VSYNC_PCR |= PORT_PCR_ISF_MASK


/******************************************************************************/
// Define
//#define INTERRUPT_PIT0_IRQ      (68) // PIT0
//
///* Set Interrupt Vector Table Function Pointers */
//#undef  VECTOR_084    //INTERRUPT_PIT0_IRQ + 16
//#define VECTOR_084  pit0_isr
#define INTERRUPT_PIT0_IRQ      (68) // PIT0

/* Set Interrupt Vector Table Function Pointers */
#undef  VECTOR_084    //INTERRUPT_PIT0_IRQ + 16
#define VECTOR_084  pit0_isr

// Function prototypes
void enable_irq(int);
void disable_irq(int);
void set_irq_priority(int, int);
void interrupt_init(void);

/******************************************************************************/
#endif /* __INTERRUPT_H__ */