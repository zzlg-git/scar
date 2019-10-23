#define SPI_SLAVE_START()   SET_BIT(SPI_CP1_PDOR, SPI_CP1_PIN);Delay_T3_uS(3)
#define SPI_SLAVE_END()     CLR_BIT(SPI_CP1_PDOR, SPI_CP1_PIN)

#include "common.h"
#include "schedule.h"
#include "Attitude_Calculation.h"

/******************************************************************************/
/******************************D E F I N I T I O N ****************************/
//===============================������================================//
vuint8                 IsGyroOffsetReset = 0;     /////�����Ҫ������������Ʈ�����򽫸ı�����Ϊ   1
/////��1�ķ�ʽ����ͨ�� �������߱�Ĳ���
Datadef                BMX055_data;
EulerAngleTypedef      SystemAttitude;            /////��̬��
EulerAngleTypedef      SystemAttitudeRate;        /////��̬���ٶ�
AttitudeDatatypedef    GyroOffset;
//===============================���ݲɼ�================================//
int          g_Line;                                                    // ����ͷ����
int          g_ImageLine;                                               //ͼ����������
Video_state  g_VideoFlag = VIDEO_WAIT;                                  //����ͼ��״̬��־
int          g_Display;                                                 //ϵͳ��ʱ
uint8        *p = NULL;                                                 //ͼ��ɼ���ָ��
int          g_LineToDiv;                                               //���вɼ��ó���
int          img_Cnt;
int          g_count_r;
int          g_count_l;
int STATUS_0;
int STATUS_1;
int STATUS_2;
int STATUS_3;
int Middle_Angle = 0;
//==============================ͼ������================================//
Int16_point  g_CenterPosition[MAX_POINT_NUM];                           //������
Int16_point  g_LeftEdge[MAX_POINT_NUM], g_RightEdge[MAX_POINT_NUM];     //���ұ���
uint8 	video_ori[MAX_VIDEO_LINE][MAX_VIDEO_POINT] = {0};
int          g_CenterNum;                                               //���ĵ����
int          g_LeftEdgeNum, g_RightEdgeNum;                             //���ұ��ص����
INT32U Temp = 0;                       //�ɼ��û������

uint16  g_Leftline[MAX_POINT_NUM], g_Rightline[MAX_POINT_NUM];///���ұ߼�¼

//==============================ͼ�����ñ���==========================//
uint16       g_Start[10], g_End[10], g_SEnum;
uint8        g_SearchFlag;
int          g_Cover;
uint8        g_CoverIndex[10],g_CoverNum;
int          g_Cover, g_Num[100];

int          fis_Sp = 0; //�жϵ�ǰ�Ƿ������
int          bis_Sp = 0; //�ж�֮ǰ�Ƿ������
int          sp_Num = 0; //�ж��ǵڼ��ξ������

int          is_Sp = 0; //�жϵ�ǰ�Ƿ������

//==============================�����ñ���==============================//
int     g_DirectionControl = MID_VIDEO_POINT;
int     g_FormerDirectionControl = MID_VIDEO_POINT;
int     g_DirectionControlWhole;
int     g_DirectionControlLine;
int     g_pulse;                                                       //�ջ�PID���Ƶķ���ֵ
int     g_PwmServo;
int     g_PwmMotor;

float bias = 0;
//==============================���ݴ���===============================//
INT8U   g_SendPic = 0; //��ͼ_��һ
INT16U  g_SendIndex;
INT8U   g_SendCurve = 0;      

void GyroOffset_init(void)      /////////��������Ʈ��ʼ��
{
  static uint16 Count = 0;
  if(Count == 0)
  {
    GyroOffset.Xdata = 0;
    GyroOffset.Ydata = 0;
    GyroOffset.Zdata = 0;
  }
  
  if(Count == 1000)
  {
    GyroOffset.Xdata /= 1000;
    GyroOffset.Ydata /= 1000;
    GyroOffset.Zdata /= 1000;
    IsGyroOffsetReset = 0;
    //DataSave();
    Count = 0;
  }
  else
  {
    BMX055_DataRead(&BMX055_data, 0);
    GyroOffset.Xdata += BMX055_data.GYROXdata;
    GyroOffset.Ydata += BMX055_data.GYROYdata;
    GyroOffset.Zdata += BMX055_data.GYROZdata;
    Count++;
  }
}

/******************************************************************************/
/******************************F U N C T I O N S  *****************************/
/******************************************************************************/
/*
* Schedule�б�����ʼ��
*/
void ScheduleInit(void)
{
    
    Sensor_Init();//��������ʼ��
    Running_Init();//���в�����ʼ��

    //���뿪�ز���
    STATUS_0 = REED0_STATUS;
    STATUS_1 = REED1_STATUS;
    STATUS_2 = REED2_STATUS;
    STATUS_3 = REED3_STATUS;

    g_Line = 0;
    //TIME0_interrupt_DIS(); 			 	//����жϷ������ر�,PIT2
    //TIME0_RESET(); 
    ENABLE_INTERRUPT_HREF();
    ENABLE_INTERRUPT_VSYNC();
}

void Running(void)
{
    //============================�ɼ�����������==============================//   
    Search();
}


/*
 * vsync_isr���ж�
 */
void vsync_isr(void){
    CLR_FLAG_VSYNC();  	
    										//������жϱ�־���������ж�
    g_Line = 0;   
  //if(g_Line>400)g_Line=0;
    ENABLE_INTERRUPT_HREF();

//  ������
    if(img_Cnt %5 ==0)
    { 
      LED_1_REV();
    }
    img_Cnt++;

}

/*
 * href_isr���ж�
 */
void href_isr(void){
    CLR_FLAG_HREF();	

    //=================================���вɼ�===============================//
    g_Line++;
    
    
    if (VIDEO_WAIT == g_VideoFlag)
    {
        if (g_Line >= START_VIDEO_LINE)
        {
            g_VideoFlag = VIDEO_ACQUISITION;
            g_ImageLine = 0;
            g_LineToDiv = 3;
        }
    }
    //���ݲɼ�״̬
    else if (VIDEO_ACQUISITION == g_VideoFlag)
    {
        if (g_LineToDiv == 3 && g_ImageLine > 20)   //���вɼ���ʽ��������20���Ǹ�1�в�һ�� ����40���Ǹ�2�в�һ��
            g_LineToDiv = 2;
        if(g_Line % g_LineToDiv == 0)
        {
            
           // Delay_T3_uS(13); 
           // DMA_EN(DMA_CHx);
            int i;
            Delay_T3_uS(7);
            p = video_ori[g_ImageLine];
            
            for(i = 0; i < MAX_VIDEO_POINT; i++)   //����ʽͼ��ɼ�
            {
                Temp = (VIDEO_PORT)>>8;
                p[i] = (uint8)Temp;
                for (int m = 0; m < 3; m ++)
                {
                    asm("nop");//Խ������Խ��
                }
                 //����Ҳ����д����g_VideoImageData[g_VideoImageLine][i]=(VIDEO_PORT);������Լ�����������д��������
            }
            
            
            g_ImageLine ++;
        }
        if(g_ImageLine >= MAX_VIDEO_LINE)
        {
            g_VideoFlag = VIDEO_SEARCH;
            
        }
    }
    //ͼ����Ϳ���״̬
    else if (VIDEO_SEARCH == g_VideoFlag)       //�ɼ����� ��ʼ��ͼ����д���
    {
        ;
    }
    //ͬ��״̬
    else if (VIDEO_FINISH == g_VideoFlag)       //������� �ȴ�ͬ����һ��ͼ��
    {
            DISABLE_INTERRUPT_HREF();					//�������ж�
            ENABLE_INTERRUPT_VSYNC();					//���ж�ʹ��
            g_VideoFlag = VIDEO_WAIT;
    }
   
    enable_irq(INTERRUPT_PIT0_IRQ);			
    
}

void Search(void)
{
  //=======================ͼ��ʶ��=========================//
  while(g_VideoFlag != VIDEO_SEARCH);
  int i, line, white_width = 0;
  
  //=======================ͼ��ʶ��=========================//
  
  g_CenterNum =  g_LeftEdgeNum = g_RightEdgeNum = 0;
  g_SearchFlag = 1;
  for (line = 0; line <= MAX_VIDEO_LINE - 1; line ++)
  {
    //���� ����������а׿��λ��
    g_SEnum = 0;  //���еİ׿����������
    if (video_ori[line][MAX_VIDEO_POINT-1] > IMAGE_MIDDLE)  //������ֵΪ�� С����ֵΪ��
      g_Start[g_SEnum] = 0;
    for (i = MAX_VIDEO_POINT-1; i > 0; i --)
    {
      if (video_ori[line][i] > IMAGE_MIDDLE && video_ori[line][i-1] <= IMAGE_MIDDLE) //��
      {
        g_End[g_SEnum] = MAX_VIDEO_POINT-i;
        g_SEnum ++;
      }
      else if (video_ori[line][i] <= IMAGE_MIDDLE && video_ori[line][i-1] > IMAGE_MIDDLE) //��
      {
        g_Start[g_SEnum] = MAX_VIDEO_POINT-i-1;
      }
      else if (g_SEnum >= 10)
      {
        break;
      }
    }   
    
    if (video_ori[line][0] > IMAGE_MIDDLE && g_SEnum < 10)
    {
      g_End[g_SEnum] = MAX_VIDEO_POINT-1;
      g_SEnum ++;
    }
    
    if(g_SEnum >= 8)
    {
        if(g_time > 3000) 
            is_Sp = 1; 
    }
        //�����������һ�е����ó������� ��Ϊ��������Ļ���
    if (g_SearchFlag)                    //����һ�д����־
    {
      int temp_mark = 0;
      if (g_SEnum == 0)
      {
        continue;
      }
      white_width = g_End[0] - g_Start[0];
      for (i = 1; i < g_SEnum; i ++)  //ֱ�������İ׿�
      {
        if (g_End[i] - g_Start[i] >= white_width)
        {
          white_width = g_End[i] - g_Start[i];
          temp_mark = i;
        }
      }
      if (white_width > WIDTH)
      {
        g_SearchFlag = 0;
        //��Ϊ��һ�����İ׿��������׿�
        g_LeftEdge[g_LeftEdgeNum].x = line;
        g_LeftEdge[g_LeftEdgeNum++].y = g_Start[temp_mark];
        g_RightEdge[g_RightEdgeNum].x = line;
        g_RightEdge[g_RightEdgeNum++].y = g_End[temp_mark];
      }
    }
    else
    {
      if (g_SEnum == 0)
      {
        break;
      }
      for (i = 0,g_CoverNum = 0; i < g_SEnum; i ++)
      {
        //���ǹ�ϵ����Ϊͼ����һ����ͨ���� ����������һ���Ѿ��ҵ��׿�����ͨ�İ׿�
        g_Cover = MIN(g_End[i],g_RightEdge[g_RightEdgeNum-1].y) - MAX(g_Start[i],g_LeftEdge[g_LeftEdgeNum-1].y);
        if (g_Cover > -1)
        {
          g_CoverIndex[g_CoverNum] = i;
          g_CoverNum ++;
        }
      }
      if (g_CoverNum == 1) //���ֻ��һ����ͨ�� ֱ��ȡ�������ͨ�׿�
      {
        g_LeftEdge[g_LeftEdgeNum].x = line;
        g_LeftEdge[g_LeftEdgeNum++].y = g_Start[g_CoverIndex[0]];
        g_RightEdge[g_RightEdgeNum].x = line;
        g_RightEdge[g_RightEdgeNum++].y = g_End[g_CoverIndex[0]];
      }
      else if (g_CoverNum == 0)  //û����ͨ�İ׿� ˵��ͼ���Ѿ��������
      {
        break;                 //��������ͼ�����ѭ�� ���������Ĳ���
      }
      else //�ж���׿�   ��Ҫȡ��
      {
        //ȡ�������������еİ׿�����ģ���Ѱ������һ�а׿������������һ���׿�
        int temp_mark, temp_dis, temp_center, last_center;
        temp_mark = 0;
        temp_center = (g_Start[g_CoverIndex[0]] + g_End[g_CoverIndex[0]]) / 2;
        last_center = (g_LeftEdge[g_LeftEdgeNum-1].y + g_RightEdge[g_RightEdgeNum-1].y) / 2;
        temp_dis = ABS(last_center - temp_center);
        for (i = 1; i < g_CoverNum; i ++)
        {
          temp_center = (g_Start[g_CoverIndex[i]] + g_End[g_CoverIndex[i]]) / 2;
          if (ABS(temp_center-last_center) < temp_dis)
          {
            temp_dis = ABS(temp_center-last_center);
            temp_mark = i;
          }
        }
        g_LeftEdge[g_LeftEdgeNum].x = line;
        g_LeftEdge[g_LeftEdgeNum++].y = g_Start[temp_mark];
        g_RightEdge[g_RightEdgeNum].x = line;
        g_RightEdge[g_RightEdgeNum++].y = g_End[temp_mark];
      }
    }
  }
  //=======================����ж�========================//
  //=======================ʮ�ִ���========================//
  //=======================������ȡ========================//
  /*
  *
  �˴���������ȡ�ǳ��򵥴ֲڣ���Ϊ֮ǰ�ڱ���������ʱ��û���������ߣ�
  ����ֱ�Ӱ�һ���׿��start��Ϊ����ߣ�end��Ϊ���ұ� ����������������
  �ߵ�����ȣ�������λ��ֱ�������ұ���ӳ�2����
  *
  */
  g_CenterNum = g_RightEdgeNum;
  for (i = 0; i < g_RightEdgeNum; i ++){
    g_CenterPosition[i].x = g_RightEdge[i].x;
    g_CenterPosition[i].y = (g_RightEdge[i].y + g_LeftEdge[i].y) / 2;
  }
  
  //===================ת���Լ��������====================//
  //��Ȩƽ�����������
  g_DirectionControlWhole = 0;
  g_DirectionControlLine = 0;
  for (i = 0; i < g_CenterNum; i ++)
  {
    if (g_CenterPosition[i].y >= 0 && g_CenterPosition[i].y <= MAX_VIDEO_POINT)
    {
      g_DirectionControlLine += (int)g_CenterPosition[i].x;
      g_DirectionControlWhole += (int)g_CenterPosition[i].y * g_CenterPosition[i].x;  //ע�����ݲ�Ҫ���    
    }
  }
  if (g_DirectionControlLine > 0)
  {
    g_DirectionControl = g_DirectionControlWhole / g_DirectionControlLine;
  }
  //��������޶�
  if (g_DirectionControl < 0)
  {
    g_DirectionControl = 0;
  }
  else if (g_DirectionControl > MAX_VIDEO_POINT)
  {
    g_DirectionControl = MAX_VIDEO_POINT;
  }
  if (ABS(g_DirectionControl - g_FormerDirectionControl) > 90)
  {
    g_DirectionControl = g_FormerDirectionControl;
  }
  
  //==================����λ������ͼ��=======================//
  if (g_SendPic)
  {
    // ͷͬ��
    uart_putvec("\xfe\x01\xfe\x01\xfe", 5);
    
    int16 k;
    // �����ܳ�
    k = 14 + MAX_VIDEO_LINE*MAX_VIDEO_POINT + 2 + MAX_POINT_NUM*8+4 + MAX_POINT_NUM*4+2 + 5;
    uart_putchar(k % 256);uart_putchar(k / 256);
    // ���
    k = g_SendIndex;
    uart_putchar(k % 256);uart_putchar(k / 256);
    // ����
    k = MAX_VIDEO_LINE;
    uart_putchar(k % 256);uart_putchar(k / 256);
    // ����
    k = MAX_VIDEO_POINT;
    uart_putchar(k % 256);uart_putchar(k / 256);
    // ͼ����ֵ
    k = IMAGE_MIDDLE;
    uart_putchar(k % 256);uart_putchar(k / 256);
    // ���ص���
    k = MAX_POINT_NUM;
    uart_putchar(k % 256);uart_putchar(k / 256);
    // ���ĵ���
    k = MAX_POINT_NUM;
    uart_putchar(k % 256);uart_putchar(k / 256);
    
    // ͼ������
    uart_putvec(&video_ori[0][0], MAX_VIDEO_LINE*MAX_VIDEO_POINT);
    k = g_DirectionControl; // ��������
    uart_putchar(k % 256);uart_putchar(k / 256);
    
    // ���ص�
    k = g_LeftEdgeNum; // �������Ч����
    uart_putchar(k % 256);uart_putchar(k / 256);
    for(i=0; i<MAX_POINT_NUM; i++)
    {
      uart_putvec((INT8U*)&g_LeftEdge[i].x, 2);// x
      uart_putvec((INT8U*)&g_LeftEdge[i].y, 2);// y
    }
    k = g_RightEdgeNum; // �ұ�����Ч����
    uart_putchar(k % 256);uart_putchar(k / 256);
    for(i=0; i<MAX_POINT_NUM; i++)
    {
      uart_putvec((INT8U*)&g_RightEdge[i].x, 2);// x
      uart_putvec((INT8U*)&g_RightEdge[i].y, 2);// y
    }
    
    // ���ĵ�
    k = g_CenterNum; // ���ĵ���Ч����
    uart_putchar(k % 256);uart_putchar(k / 256);
    for(i=0; i<MAX_POINT_NUM; i++)
    {
      uart_putvec((INT8U*)&g_CenterPosition[i].x, 2);// x
      uart_putvec((INT8U*)&g_CenterPosition[i].y, 2);// y
    }
    
    // ��������
    uart_putvec("\x15\x16\x17\x18\x19", 5);
    
    // βͬ��
    uart_putvec("\xfd\x02\xfd\x02\xfd", 5);
    
    g_SendIndex ++;
  }
  
  //======================״̬ת��=========================//
  
  g_VideoFlag = VIDEO_FINISH;
  //ENABLE_INTERRUPT_IRQ();   //���ж�ʹ��
  
  //�궨ʱ��
  if (g_Display % 5 == 0)
  {
    LED_1_REV();
  }
}

//��ʱʱ��
int16 g_time = 0;

//�ٿر���
uint8 g_speed_count;
int16 g_speed_num = 20;//ÿ20���ж��ٿ�һ��

//stand����
float g_kgyo;
float g_b_tg;
float g_stand_p;
float g_stand_d;

//run����
float g_speed_integral;
float g_speed_p;
float g_speed_i;
float speed;
float speed_feedback;
float speed_feedback_pre;

//turn����
float g_direction_p;
float g_direction_d;
float g_biaslast = 0; 

//�������������
int16 g_countervalue_l = 0;
int16 g_countervalue_r = 0;
float g_feedback_l = 0;
float g_feedback_r = 0;

//���ҵ��������
int16 g_motor_l,g_motor_r;

//�ٶȸ���
int16  g_speed_final;

//��״̬��
CarStatus carsta;

//��������
CarControll carcon;

/******************************************************************************/
unsigned char IsSend = 0;
float AccZAngle = 0;



void pit0_isr(void)
{
  PIT_TFLG0|=PIT_TFLG_TIF_MASK;
 
  //��ʱ����
  TIME0_RESET();
  g_countervalue_r = -Get_Counter_Value(2);
  g_countervalue_l = -Get_Counter_Value(1); 
  
  //�ٶȷ�������
  g_feedback_l += g_countervalue_l;
  g_feedback_r += g_countervalue_r;
  
  TIME1_RESET();
  Sensor_Process();
  
  /******************************************
  �����ǡ��������������ɼ�����
  ******************************************/  
  //����������
  Car_Stand(); //ֱ������ 
  Car_Run(); //�ٶȿ���  
  Car_Turn(); //ת�����
  
  //�����ֵ
  Motor_Set(); //�����P��
  
  //���ڵ���
  if(g_SendCurve)
    uart_test();  

  g_time++;
  
} // �����ж�
 

/*
function name:  running_init
description:    init
*/
void Running_Init(void)
{
    //ֱ������
    g_stand_p = 50;
    g_stand_d = 0.03;
    
    //Ԥ����ֵ
    Middle_Angle = 73;

    //�ٶȲ���
    g_speed_final = 300;
    g_speed_p = 1;
    g_speed_i = 0.1;
    
    //ת�����
    g_direction_p = 6;
    g_direction_d = 0.1;

}

void Car_Stand(void)
{ 
  carcon.stand = (CarSta.Pitch - Middle_Angle) * g_stand_p + CarSta.PitchRate * g_stand_d; 
}

void Car_Run(void)
{
  if(g_time < 300) 
      carcon.run_set = 0; //ǰ1.5�벻���ٶ�
  
  //��200��ʹ�ٶȴﵽ�趨ֵ
  carcon.run_set += (g_speed_final - carcon.run_set)/200;

  carcon.run = (carcon.run_new - carcon.run_pre) * g_speed_count / g_speed_num + carcon.run_pre;
  
  g_speed_count ++;

  //p ��ǰ�㣬���أ���p����������̬���
  if(g_speed_count > g_speed_num)
  {
    speed_feedback = carcon.run_set - (g_feedback_l+g_feedback_r)/2;
    
    speed_feedback_pre = speed_feedback;
    
    g_speed_integral += speed_feedback * g_speed_i;
    
    //�����޷�������
    if(g_speed_integral >300)
      g_speed_integral =300;
    else if(g_speed_integral < -100)
      g_speed_integral = -100;
    
    
    speed = speed_feedback * g_speed_p + g_speed_integral ;
    
    carcon.run_pre = carcon.run_new;
    carcon.run_new = speed;
    
    g_speed_count = 1;
    g_feedback_l = 0;
    g_feedback_r = 0;   
    
    
  }
}

/*
function name:  car_turn
parameters:     none
author:         Xian Lee
date:           2012-6-27
description:    control car turn
*/
void Car_Turn(void)
{
    float anglev =  CarSta.YawRate;
    
    bias = MID_VIDEO_POINT - g_DirectionControl;
    
    carcon.turn =  - bias * g_direction_p + anglev * g_direction_d;
    
}

void Motor_Set(void)
{                                                                                                                                 
    //�����ں�
    //carcon.motor_left  = carcon.stand - carcon.run + carcon.turn ;
    //carcon.motor_right = carcon.stand - carcon.run - carcon.turn ;
    carcon.motor_left  = carcon.stand ;
    carcon.motor_right = carcon.stand ;
  
    //�ٶȱ���
    if(carcon.motor_left > 500) 
        g_motor_l = 500;
    else if(carcon.motor_left < -500) 
        g_motor_l = -500;
    else 
        g_motor_l = (int16)carcon.motor_left;
    
    if(carcon.motor_right > 500) 
        g_motor_r = 500;
    else if(carcon.motor_right < -500) 
        g_motor_r = -500;
    else 
        g_motor_r = (int16)carcon.motor_right;
    
    //�Ƕȱ���
    //if(g_time > 2000)  //�������Ƕ�
    //{
    //    g_motor_l = 0;
     //   g_motor_r = 0;
   // }
        
    SET_PWM_MOTL(-g_motor_l);
    SET_PWM_MOTR(g_motor_r);

    
}


/*
function name:  uart_test
parameters:     none
author:         How
date:           2012-6-28
description:    uart test
*/
void uart_test(void)
{ 
    uint16 temp;
    int16 tempp;
    
    uart_putchar(0xA5);
    
    //check
    temp = 0;
    uart_putchar((uint8)(temp>>8));
    uart_putchar((uint8)(temp));
    

    //car_angle:����ռ�����ֽڵ��˲��Ƕȣ�carsta.car_angle��
    tempp = (int16)carsta.car_angle;
    temp = tempp;
    uart_putchar((uint8)(temp>>8));
    uart_putchar((uint8)(temp));
                 
    //angle_v������ռ�����ֽڵ�������ֵ��Gyro.angle_v_X��
    tempp = (int16) CarSta.Pitch;
    temp = tempp;
    uart_putchar((uint8)(temp>>8));
    uart_putchar((uint8)(temp));
    
    //angle_z������ռ�����ֽڵļ��ٶȼ�ֵ��Acc.Zdata - Acc.middle_Z��
    tempp = (int16)( CarSta.PitchRate);
    temp = tempp;
    uart_putchar((uint8)(temp>>8));
    uart_putchar((uint8)temp);
    
    
    uart_putchar(0xAA);
}
