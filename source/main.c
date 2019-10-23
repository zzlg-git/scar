#include "common.h"
int testleft = 100;
int testright = 100;

void main(void)
{
  
  DisableInterrupts();      //关闭中断
  
  system_init();            //系统初始化

  ScheduleInit();           //控制参数赋初值
  
 EnableInterrupts();       //打开中断
  
  for(;;)
  {
    Running();

//    在线调试、看轮子转的方向与想象的是否相同
    //SET_PWM_MOTL(-testleft);//负前
    //SET_PWM_MOTR(testright);//正前
  }
}

