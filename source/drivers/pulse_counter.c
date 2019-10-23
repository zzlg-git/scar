/*
 * File:	pulse_counter.c
 * Purpose:	pulse_counter
 *
 * Notes:		
 */
#include "common.h"
#include "pulse_counter.h"

/******************************************************************************/
void Pulse_Counter_Init(void)
{
  #if(0 == PULSE_COUNTER1_FTM)
    SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK; // open the clock gate to the FTM1 module
  #elif(1 == PULSE_COUNTER1_FTM)
      SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;
  #elif(2 == PULSE_COUNTER1_FTM)
      SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;
  #else
  #error "Definition PULSE_COUNTER1_FTM ERROR ! "
  #endif
      
  #if(0 == PULSE_COUNTER2_FTM)
    SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK; // open the clock gate to the FTM1 module
  #elif(1 == PULSE_COUNTER2_FTM)
      SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;
  #elif(2 == PULSE_COUNTER2_FTM)
      SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;
  #else
  #error "Definition PULSE_COUNTER2_FTM ERROR ! "
  #endif
      
    FTM_MODE_REG(PULSE_COUNTER1_PTR) = 0|FTM_MODE_WPDIS_MASK|FTM_MODE_FTMEN_MASK; // Write protection is disabled, FTM enabale
    FTM_CNTIN_REG(PULSE_COUNTER1_PTR) = 0UL; // counter initialization value is 0
    FTM_CNT_REG(PULSE_COUNTER1_PTR) = 0UL;   //clear counter
    FTM_MOD_REG(PULSE_COUNTER1_PTR) = FTM_MOD_MOD(0xffff);  //Maxvalue 65535
    FTM_SC_REG(PULSE_COUNTER1_PTR) = 0|FTM_SC_CLKS(3)|FTM_SC_PS(1);   //external clock,divide by 2
    FTM_QDCTRL_REG(PULSE_COUNTER1_PTR) = 0|FTM_QDCTRL_QUADEN_MASK|FTM_QDCTRL_PHAFLTREN_MASK|FTM_QDCTRL_PHBFLTREN_MASK;
    FTM_FILTER_REG(PULSE_COUNTER1_PTR) = 0|FTM_FILTER_CH0FVAL(15)|FTM_FILTER_CH1FVAL(15);
    
    PULSE_COUNTER1A_PCR = 0 | PORT_PCR_MUX(PULSE_COUNTER1A_ALTn);
    PULSE_COUNTER1B_PCR = 0 | PORT_PCR_MUX(PULSE_COUNTER1B_ALTn); 
    
    FTM_MODE_REG(PULSE_COUNTER2_PTR) = 0|FTM_MODE_WPDIS_MASK|FTM_MODE_FTMEN_MASK; // Write protection is disabled, FTM enabale
    FTM_CNTIN_REG(PULSE_COUNTER2_PTR) = 0UL; // counter initialization value is 0
    FTM_CNT_REG(PULSE_COUNTER2_PTR) = 0UL;  //clear counter
    FTM_MOD_REG(PULSE_COUNTER2_PTR) = FTM_MOD_MOD(0xffff);  //Maxvalue 65535
    FTM_SC_REG(PULSE_COUNTER2_PTR) = 0|FTM_SC_CLKS(3)|FTM_SC_PS(1);   //external clock,divide by 2
    FTM_QDCTRL_REG(PULSE_COUNTER2_PTR) = 0|FTM_QDCTRL_QUADEN_MASK|FTM_QDCTRL_PHAFLTREN_MASK|FTM_QDCTRL_PHBFLTREN_MASK;
    FTM_FILTER_REG(PULSE_COUNTER2_PTR) = 0|FTM_FILTER_CH0FVAL(15)|FTM_FILTER_CH1FVAL(15);
    
    PULSE_COUNTER2A_PCR = 0 | PORT_PCR_MUX(PULSE_COUNTER2A_ALTn);
    PULSE_COUNTER2B_PCR = 0 | PORT_PCR_MUX(PULSE_COUNTER2B_ALTn); 
}
/******************************************************************************/
/**/
int16 Get_Counter_Value(int CounterNo)
{
  int16 countervalue;
  if(1==CounterNo)
  {
    countervalue = (int16)FTM_CNT_REG(PULSE_COUNTER1_PTR);   //Read counter value 
    FTM_CNT_REG(PULSE_COUNTER1_PTR) = 0UL;
  }
  else
  {
    countervalue = (int16)FTM_CNT_REG(PULSE_COUNTER2_PTR);  //Read counter value
    FTM_CNT_REG(PULSE_COUNTER2_PTR) = 0UL;
  }
  
  return countervalue;
}
/******************************************************************************/

