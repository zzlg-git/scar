#include "common.h"
int testleft = 100;
int testright = 100;

void main(void)
{
  
  DisableInterrupts();      //�ر��ж�
  
  system_init();            //ϵͳ��ʼ��

  ScheduleInit();           //���Ʋ�������ֵ
  
 EnableInterrupts();       //���ж�
  
  for(;;)
  {
    Running();

//    ���ߵ��ԡ�������ת�ķ�����������Ƿ���ͬ
    //SET_PWM_MOTL(-testleft);//��ǰ
    //SET_PWM_MOTR(testright);//��ǰ
  }
}

