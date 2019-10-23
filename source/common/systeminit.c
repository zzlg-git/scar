/*
 * File:	systeminit.c
 * Purpose: system initialization
 *
 * Notes:		
 */
#include "common.h"
#include "systeminit.h"

/******************************************************************************/
void system_init(void)
{ 
    timer_init();
    Delay_T3_mS(50);
    gpio_init();
    Delay_T3_mS(50);
   // i2c_init();
    if(sccb_init()==0)
    {
//      while(1);
    }
   // sccb_init();
    interrupt_init();
    Pulse_Counter_Init();
    Pwm_Init();
    
    SET_PWM_MOTL(0); 
    SET_PWM_MOTR(0);
    
    //spi_init();
    ADC_Init();
    Delay_T3_mS(50);
   // init_spisoft();
   // MPU9250_Init_spisoft();  
    Delay_T3_mS(5);
    uart_init();
    IIC_init();
    Delay_T3_mS(5);
    BMX055_init();
} // system_init
/******************************************************************************/
