/*
* File:	keyboard.c
* Purpose: keyboard ZLG7290
*
* Notes:
*/
#include "common.h"
#include "keyboard_oled.h"
#include "schedule.h"
INT8U Exposure= 0;


void *malloc(unsigned int );
int free();
 
int ZLG7290GetKey(void)         //*号键为输入-1
{
  uint8 i,tmp = 0;
  int rt = 0;
  KShowLineInputClr();
  for(i = 0;i < 8;)
  {
    while(ZLG7290_INT()==1)
    {
      ;
    }
    tmp = CH455_Key();
    if(tmp == 0x0a)
    {
      i=0;
      rt = -1;
      return rt;
    }
    else if(tmp == 0x0b) return rt;
    else
    {
      if(rt * 10 + tmp > 65000)
      {
        continue;
      }
      rt = rt * 10 + tmp;
      i++;
      KShowLineNumInput(rt);
    }
    while(!(ZLG7290_INT()==1));
  }
  return rt;
}



uint16 ZLG7290GetKeyAndDisplay(void)
{
    uint8 i,tmp = 0;
    uint16 rt = 0;
	
	KShowLineInputClr();
    
    for(i = 0;i < 8;)
    {
        while(ZLG7290_INT()==1);
        tmp = CH455_Key(); 
        if(tmp == 0x0a)
        {
            i=0;
            rt = 0;
            KShowLineInputClr();
            while(!(ZLG7290_INT()==1));
            continue;
        }
        else if(tmp == 0x0b) return rt;
        else
        {
            if(rt * 10 + tmp > 65000)
            {
                continue;
            }
            rt = rt * 10 + tmp;
            i++;
            KShowLineNumInput(rt);
        }
        while(!(ZLG7290_INT()==1));
    }
    
    return rt;
}

INT16U VCC;
void Choise(void)
{
  	//初始化
    Delay_T3_mS(150);
    Init_OLED();
    Init_CH455(); 
    OLED_Reset(0x00);
    OLED_RAM_Fill_add(0);  
    OLED_RAM_Refresh_add();
   
    
    INT16U choise;
    Delay_T3_mS(50);
   Flash_read_All();  
    
    for (;;)
    {
      OLED_RAM_Fill_add(0);  
      OLED_RAM_Refresh_add();
      
      VCC = Get_AD_Channel(0)*30*33/65536;
      Delay_T3_mS(10);
      KShowLineClear(1);
      KShowLineClear(2);
      KShowLineClear(3);
      KShowLineClear(4);
      char s[100];    
      
      char Ctemp[200]="  *SMARTCAR*";
      //  char Ctemp1[200]="SMARTCAR";
      	
      ShowVoltage(3,VCC);
      
      
      //  KShowLineStrInfo("Choise:");        
        choise = (ZLG7290GetKeyAndDisplay());
    
        Delay_T3_mS(10);
        
        if(choise == 0x00)         //开始
        {  
          Delay_T3_mS(50);
          
          OLED_RAM_Fill_add(0);  
          OLED_RAM_Refresh_add();
          Delay_T3_mS(50);
          break;
        }
        else if(choise==1)//面积规划参数
        {  
          Set_Path();
        }
     
       
        
    }
    Flash_write_All();
    KShowLineStr(4,"Go......");
    
    
}

