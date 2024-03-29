/*
 * File:		interrupt.c
 * Purpose:
 *
 * Notes:
 */
#include "common.h"
#include "interrupt.h"

/******************************************************************************/
/*
 * Initialize the NVIC to enable the specified IRQ.
 */
void interrupt_init(void)
{
    // PORT
//    INTERRUPT_IRQ_PCR = (0|PORT_PCR_MUX(1)|PORT_PCR_IRQC(10)); // Interrupt on falling edge.   

//    CLR_BIT(INTERRUPT_IRQ_DDR, INTERRUPT_IRQ_PIN); //       input

    // Clear the interrupt flag
//    INTERRUPT_IRQ_PCR |= PORT_PCR_ISF_MASK;
    // PORT
    INTERRUPT_VSYNC_PCR = (0|PORT_PCR_MUX(1)|PORT_PCR_IRQC(10)); // Interrupt on falling edge.  
    INTERRUPT_HREF_PCR = (0|PORT_PCR_MUX(1)|PORT_PCR_IRQC(10)); // Interrupt on falling edge.
    
    CLR_BIT(INTERRUPT_VSYNC_DDR, INTERRUPT_HREF_PIN); // input
    CLR_BIT(INTERRUPT_HREF_DDR, INTERRUPT_VSYNC_PIN);
    
    // Clear the interrupt flag
    INTERRUPT_VSYNC_PCR |= PORT_PCR_ISF_MASK;
    INTERRUPT_HREF_PCR  |= PORT_PCR_ISF_MASK;
    // set BASEPRI
    __set_BASEPRI(0xF0); /* Disable all interrupt priorities */

    // enable irq
//    enable_irq(INTERRUPT_IRQ_IRQ);
    enable_irq(INTERRUPT_PIT0_IRQ);  
    enable_irq(INTERRUPT_VSYNC_IRQ);
    enable_irq(INTERRUPT_HREF_IRQ);
    // set irq priority
//    set_irq_priority(INTERRUPT_IRQ_IRQ, 3); // higher
    set_irq_priority(INTERRUPT_PIT0_IRQ, 5);// higherd
    set_irq_priority(INTERRUPT_VSYNC_IRQ, 3); // higher //3
    set_irq_priority(INTERRUPT_HREF_IRQ, 4);//4
    
    __set_BASEPRI(0x60); // priority 6-0 are allowed.

}

/******************************************************************************/
/*
 * Initialize the NVIC to enable the specified IRQ.
 *
 * NOTE: The function only initializes the NVIC to enable a single IRQ.
 * Interrupts will also need to be enabled in the ARM core. This can be
 * done using the EnableInterrupts macro.
 *
 * Parameters:
 * irq    irq number to be enabled (the irq number NOT the vector number)
 */
void enable_irq (int irq)
{
    int div;

    /* Make sure that the IRQ is an allowable number. Right now up to 91 is
     * used.
     */
    if(irq>91 || irq<0)
        return;

    /* Determine which of the NVICISERs corresponds to the irq */
    div = irq/32;

    switch (div)
    {
        case 0x0:
            NVICICPR0 |= 1 << (irq%32);
            NVICISER0 |= 1 << (irq%32);
            break;
        case 0x1:
            NVICICPR1 |= 1 << (irq%32);
            NVICISER1 |= 1 << (irq%32);
            break;
        case 0x2:
            NVICICPR2 |= 1 << (irq%32);
            NVICISER2 |= 1 << (irq%32);
            break;
        case 0x3:
            NVICICPR3 |= 1 << (irq%32);
            NVICISER3 |= 1 << (irq%32);
            break;
    }
}

/******************************************************************************/
/*
 * Initialize the NVIC to disable the specified IRQ.
 *
 * NOTE: The function only initializes the NVIC to disable a single IRQ.
 * If you want to disable all interrupts, then use the DisableInterrupts
 * macro instead.
 *
 * Parameters:
 * irq    irq number to be disabled (the irq number NOT the vector number)
 */
void disable_irq (int irq)
{
    int div;

    /* Make sure that the IRQ is an allowable number. Right now up to 91 is
     * used.
     */
    if(irq>91 || irq<0)
        return;

    /* Determine which of the NVICICERs corresponds to the irq */
    div = irq/32;

    switch (div)
    {
        case 0x0:
            NVICICER0 |= 1 << (irq%32);
            break;
        case 0x1:
            NVICICER1 |= 1 << (irq%32);
            break;
        case 0x2:
            NVICICER2 |= 1 << (irq%32);
            break;
        case 0x3:
            NVICICER3 |= 1 << (irq%32);
            break;
    }
}

/******************************************************************************/
/*
 * Initialize the NVIC to set specified IRQ priority.
 *
 * NOTE: The function only initializes the NVIC to set a single IRQ priority.
 * Interrupts will also need to be enabled in the ARM core. This can be
 * done using the EnableInterrupts macro.
 *
 * Parameters:
 * irq    irq number to be enabled (the irq number NOT the vector number)
 * prio   irq priority. 0-15 levels. 0 max priority
 */
void set_irq_priority (int irq, int prio)
{
    /*irq priority pointer*/
    uint8 *prio_reg;

    /* Make sure that the IRQ is an allowable number. Right now up to 91 is
     * used.
     */
    if(irq>91 || irq<0 || prio>15 || prio<0)
        return;

    /* Determine which of the NVICIPx corresponds to the irq */
    prio_reg = (uint8*)(&NVICIP0 + irq);
    /* Assign priority to IRQ */
    *prio_reg = ((prio&0xF) << (8 - ARM_INTERRUPT_LEVEL_BITS));
}
/******************************************************************************/