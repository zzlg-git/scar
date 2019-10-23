#ifndef __PULSE_COUNTER_H__
#define __PULSE_COUNTER_H__

/********************************************************************/
#define PULSE_COUNTER1_FTM (1)
#define PULSE_COUNTER2_FTM (2)
#define PULSE_COUNTER1_PTR FTM1_BASE_PTR
#define PULSE_COUNTER2_PTR FTM2_BASE_PTR

#define PULSE_COUNTER1A_PCR     PORTB_PCR19
#define PULSE_COUNTER1A_ALTn    (6) 
#define PULSE_COUNTER1B_PCR     PORTB_PCR18 
#define PULSE_COUNTER1B_ALTn    (6)

#define PULSE_COUNTER2A_PCR      PORTB_PCR1
#define PULSE_COUNTER2A_ALTn    (6) 
#define PULSE_COUNTER2B_PCR     PORTB_PCR0 
#define PULSE_COUNTER2B_ALTn    (6)

#define PULSE_COUNTER1_GET(x) \
       {x = (int16)FTM_CNT_REG(PULSE_COUNTER1_PTR); \
        FTM_CNT_REG(PULSE_COUNTER1_PTR) = 0;} // Any write to CNT resets the FTM counter to the value in the CNTIN register.
#define PULSE_COUNTER2_GET(x) \
       {x = (int16)FTM_CNT_REG(PULSE_COUNTER2_PTR); \
        FTM_CNT_REG(PULSE_COUNTER2_PTR) = 0;} // Any write to CNT resets the FTM counter to the value in the CNTIN register.
// Function prototypes
void Pulse_Counter_Init(void);
int16 Get_Counter_Value(int CounterNo);
/********************************************************************/
#endif /* __PULSE_COUNTER_H__ */
