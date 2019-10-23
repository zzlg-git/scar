/*
 * File:        common.h
 * Purpose:     File to be included by all project files
 *
 * Notes:
 */
#ifndef __COMMON_H__
#define __COMMON_H__

/******************************************************************************/

/*
 * Debug prints ON (#define) or OFF (#undef)
 */
//#define DEBUG
//#define DEBUG_PRINT

/*
 * Conditional compilation config
 */


/*
 * Config Defines
 */
#define CORE_CLK_MHZ    PLL128 //128//PLL180      // 50,100,160,180,200,48,96
#define UART_BAUD       115200 //115200      // UART baud rate


/*
 * Defines used as function
 */
#define Delay_T3_nS(n)  {TIME3_RESET();while(TIME3_GET_uS()*1000<=n);}
#define Delay_T3_uS(n)  {TIME3_RESET();while(TIME3_GET_uS()<=n);}
#define Delay_T3_mS(n)  {TIME3_RESET();while(TIME3_GET_uS()<=n*1000);}
#define Common_delay(n) {TIME3_RESET();while(TIME3_GET_uS()<=n*1000);}
//#define  Delay_ms(n)    Delay_T3_mS(n)
//#define  Delay_us(n)    Delay_T3_uS(n)

/*
 * Include the cpu specific header file
 */
#include<freescale/MK60DZ10.h> // pin100
#include<c/intrinsics.h>

/*
 * Include the generic CPU header file
 */
#include "clkinit.h"
#include "core.h"
#include "start.h"
#include "vectors.h"

/*
 * Include drivers utilities
 */
#include "gpio.h"
#include "3IN1.h"
#include "interrupt.h"
#include "mcg.h"
#include "pulse_counter.h"
#include "pwm.h"
#include "timer.h"
#include "uart.h"
#include "wdog.h"
#include "adc.h"
#include "spi.h"
#include "math.h"
#include <cmath>
#include "flash.h"
#include "spisoft.h"
#include "sccb.h"
#include "iicsoft.h"
#include "BMX055.h"
#include "Attitude_Calculation.h"

/*********** MPU9250 **************/ 
#include  "MPU9250.h"
#include "sensoroperation.h"
#include "MPU9250_api.h"

/*
 * Include common utilities
 */
#include "systeminit.h"
#include "schedule.h"
#include "mathadd.h"
#include "keyboard_oled.h"
/******************************************************************************/




#endif /* __COMMON_H__ */





