/*
 * File:	pwm.h
 * Purpose:	pwm to contrl motor and servo
 *
 * Notes:
 */

#ifndef __PWM_H__
#define __PWM_H__

/******************************************************************************/

// Define
#define PWM_MOTA_PCR     PORTC_PCR3 // FTM0_CH2
#define PWM_MOTA_ALTn    (4) // 0-7
#define PWM_MOTB_PCR     PORTC_PCR1 // FTM0_CH0
#define PWM_MOTB_ALTn    (4) // 0-7
#define PWM_MOT_PTR     FTM0_BASE_PTR // FTM0_BASE_PTR - FTM2_BASE_PTR
#define PWM_MOT_FTM     (0) // 0-2
#define PWM_MOTA_CHANNEL (2) // when PWM_MOT_FTM=0 0-7, else 0-1  left motor, channel 1
#define PWM_MOTB_CHANNEL (0) // right motor, channel 2


#define SET_PWM_MOTR(x) \
       {FTM_CnV_REG(PWM_MOT_PTR,PWM_MOTB_CHANNEL) = (x/2+500) & 0xffff;\
        FTM_PWMLOAD_REG(PWM_MOT_PTR) |= FTM_PWMLOAD_LDOK_MASK;}

#define SET_PWM_MOTL(x) \
       {FTM_CnV_REG(PWM_MOT_PTR,PWM_MOTA_CHANNEL) = (x/2+500) & 0xffff;\
        FTM_PWMLOAD_REG(PWM_MOT_PTR) |= FTM_PWMLOAD_LDOK_MASK;}

// Function prototypes
void Pwm_Init(void);
void Pwm_Motor_Init(void);

/******************************************************************************/
#endif /* __PWM_H__ */
