#define SPI_SLAVE_START()   SET_BIT(SPI_CP1_PDOR, SPI_CP1_PIN);Delay_T3_uS(3)
#define SPI_SLAVE_END()     CLR_BIT(SPI_CP1_PDOR, SPI_CP1_PIN)

#include "common.h"
#include "schedule.h"
#include "Attitude_Calculation.h"

/******************************************************************************/
/******************************D E F I N I T I O N ****************************/
//===============================陀螺仪================================//
vuint8                 IsGyroOffsetReset = 0;     /////如果需要进行陀螺仪零飘矫正则将改变量置为   1
/////置1的方式可以通过 按键或者别的操作
Datadef                BMX055_data;
EulerAngleTypedef      SystemAttitude;            /////姿态角
EulerAngleTypedef      SystemAttitudeRate;        /////姿态角速度
AttitudeDatatypedef    GyroOffset;
//===============================数据采集================================//
int          g_Line;                                                    // 摄像头行数
int          g_ImageLine;                                               //图像数据行数
Video_state  g_VideoFlag = VIDEO_WAIT;                                  //处理图像状态标志
int          g_Display;                                                 //系统计时
uint8        *p = NULL;                                                 //图像采集用指针
int          g_LineToDiv;                                               //隔行采集用除数
int          img_Cnt;
int          g_count_r;
int          g_count_l;
int STATUS_0;
int STATUS_1;
int STATUS_2;
int STATUS_3;
int Middle_Angle = 0;
//==============================图像数据================================//
Int16_point  g_CenterPosition[MAX_POINT_NUM];                           //中心线
Int16_point  g_LeftEdge[MAX_POINT_NUM], g_RightEdge[MAX_POINT_NUM];     //左右边沿
uint8 	video_ori[MAX_VIDEO_LINE][MAX_VIDEO_POINT] = {0};
int          g_CenterNum;                                               //中心点个数
int          g_LeftEdgeNum, g_RightEdgeNum;                             //左右边沿点个数
INT32U Temp = 0;                       //采集用缓冲变量

uint16  g_Leftline[MAX_POINT_NUM], g_Rightline[MAX_POINT_NUM];///左右边记录

//==============================图像处理用变量==========================//
uint16       g_Start[10], g_End[10], g_SEnum;
uint8        g_SearchFlag;
int          g_Cover;
uint8        g_CoverIndex[10],g_CoverNum;
int          g_Cover, g_Num[100];

int          fis_Sp = 0; //判断当前是否是起点
int          bis_Sp = 0; //判断之前是否是起点
int          sp_Num = 0; //判断是第几次经过起点

int          is_Sp = 0; //判断当前是否是起点

//==============================控制用变量==============================//
int     g_DirectionControl = MID_VIDEO_POINT;
int     g_FormerDirectionControl = MID_VIDEO_POINT;
int     g_DirectionControlWhole;
int     g_DirectionControlLine;
int     g_pulse;                                                       //闭环PID控制的反馈值
int     g_PwmServo;
int     g_PwmMotor;

float bias = 0;
//==============================数据传输===============================//
INT8U   g_SendPic = 0; //看图_置一
INT16U  g_SendIndex;
INT8U   g_SendCurve = 0;      

void GyroOffset_init(void)      /////////陀螺仪零飘初始化
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
* Schedule中变量初始化
*/
void ScheduleInit(void)
{
    
    Sensor_Init();//传感器初始化
    Running_Init();//运行参数初始化

    //拨码开关参数
    STATUS_0 = REED0_STATUS;
    STATUS_1 = REED1_STATUS;
    STATUS_2 = REED2_STATUS;
    STATUS_3 = REED3_STATUS;

    g_Line = 0;
    //TIME0_interrupt_DIS(); 			 	//电机中断服务程序关闭,PIT2
    //TIME0_RESET(); 
    ENABLE_INTERRUPT_HREF();
    ENABLE_INTERRUPT_VSYNC();
}

void Running(void)
{
    //============================采集及搜索中心==============================//   
    Search();
}


/*
 * vsync_isr场中断
 */
void vsync_isr(void){
    CLR_FLAG_VSYNC();  	
    										//清除场中断标志，开启行中断
    g_Line = 0;   
  //if(g_Line>400)g_Line=0;
    ENABLE_INTERRUPT_HREF();

//  场序监控
    if(img_Cnt %5 ==0)
    { 
      LED_1_REV();
    }
    img_Cnt++;

}

/*
 * href_isr行中断
 */
void href_isr(void){
    CLR_FLAG_HREF();	

    //=================================分行采集===============================//
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
    //数据采集状态
    else if (VIDEO_ACQUISITION == g_VideoFlag)
    {
        if (g_LineToDiv == 3 && g_ImageLine > 20)   //分行采集方式，即上面20行是隔1行采一行 下面40行是隔2行采一行
            g_LineToDiv = 2;
        if(g_Line % g_LineToDiv == 0)
        {
            
           // Delay_T3_uS(13); 
           // DMA_EN(DMA_CHx);
            int i;
            Delay_T3_uS(7);
            p = video_ori[g_ImageLine];
            
            for(i = 0; i < MAX_VIDEO_POINT; i++)   //覆盖式图像采集
            {
                Temp = (VIDEO_PORT)>>8;
                p[i] = (uint8)Temp;
                for (int m = 0; m < 3; m ++)
                {
                    asm("nop");//越多赛道越宽
                }
                 //这里也可以写作“g_VideoImageData[g_VideoImageLine][i]=(VIDEO_PORT);”大家自己看看这两种写法的区别
            }
            
            
            g_ImageLine ++;
        }
        if(g_ImageLine >= MAX_VIDEO_LINE)
        {
            g_VideoFlag = VIDEO_SEARCH;
            
        }
    }
    //图像处理和控制状态
    else if (VIDEO_SEARCH == g_VideoFlag)       //采集结束 开始对图像进行处理
    {
        ;
    }
    //同步状态
    else if (VIDEO_FINISH == g_VideoFlag)       //处理结束 等待同步下一场图像
    {
            DISABLE_INTERRUPT_HREF();					//禁用行中断
            ENABLE_INTERRUPT_VSYNC();					//场中断使能
            g_VideoFlag = VIDEO_WAIT;
    }
   
    enable_irq(INTERRUPT_PIT0_IRQ);			
    
}

void Search(void)
{
  //=======================图像识别=========================//
  while(g_VideoFlag != VIDEO_SEARCH);
  int i, line, white_width = 0;
  
  //=======================图像识别=========================//
  
  g_CenterNum =  g_LeftEdgeNum = g_RightEdgeNum = 0;
  g_SearchFlag = 1;
  for (line = 0; line <= MAX_VIDEO_LINE - 1; line ++)
  {
    //首先 求出本行所有白块的位置
    g_SEnum = 0;  //本行的白块计数器清零
    if (video_ori[line][MAX_VIDEO_POINT-1] > IMAGE_MIDDLE)  //大于阈值为白 小于阈值为黑
      g_Start[g_SEnum] = 0;
    for (i = MAX_VIDEO_POINT-1; i > 0; i --)
    {
      if (video_ori[line][i] > IMAGE_MIDDLE && video_ori[line][i-1] <= IMAGE_MIDDLE) //右
      {
        g_End[g_SEnum] = MAX_VIDEO_POINT-i;
        g_SEnum ++;
      }
      else if (video_ori[line][i] <= IMAGE_MIDDLE && video_ori[line][i-1] > IMAGE_MIDDLE) //左
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
        //把最下面的那一行单独拿出来处理 做为后续处理的基础
    if (g_SearchFlag)                    //最下一行处理标志
    {
      int temp_mark = 0;
      if (g_SEnum == 0)
      {
        continue;
      }
      white_width = g_End[0] - g_Start[0];
      for (i = 1; i < g_SEnum; i ++)  //直接求最宽的白块
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
        //认为第一行最宽的白块便是所需白块
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
        //覆盖关系，因为图像是一个联通区域 于是找与上一行已经找到白块相联通的白块
        g_Cover = MIN(g_End[i],g_RightEdge[g_RightEdgeNum-1].y) - MAX(g_Start[i],g_LeftEdge[g_LeftEdgeNum-1].y);
        if (g_Cover > -1)
        {
          g_CoverIndex[g_CoverNum] = i;
          g_CoverNum ++;
        }
      }
      if (g_CoverNum == 1) //如果只有一个联通的 直接取出这个联通白块
      {
        g_LeftEdge[g_LeftEdgeNum].x = line;
        g_LeftEdge[g_LeftEdgeNum++].y = g_Start[g_CoverIndex[0]];
        g_RightEdge[g_RightEdgeNum].x = line;
        g_RightEdge[g_RightEdgeNum++].y = g_End[g_CoverIndex[0]];
      }
      else if (g_CoverNum == 0)  //没有联通的白块 说明图像已经处理完毕
      {
        break;                 //跳出整个图像处理的循环 进入求中心部分
      }
      else //有多个白块   需要取舍
      {
        //取舍规则：先求出本行的白块的中心，再寻找与上一行白块中心最相近的一个白块
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
  //=======================起点判断========================//
  //=======================十字处理========================//
  //=======================中心求取========================//
  /*
  *
  此处的中心求取非常简单粗糙，因为之前在边沿搜索的时候并没有区分两边，
  而是直接把一个白块的start认为是左边，end认为是右边 故中心数将与左右
  边的数相等，而中心位置直接由左右边相加除2所得
  *
  */
  g_CenterNum = g_RightEdgeNum;
  for (i = 0; i < g_RightEdgeNum; i ++){
    g_CenterPosition[i].x = g_RightEdge[i].x;
    g_CenterPosition[i].y = (g_RightEdge[i].y + g_LeftEdge[i].y) / 2;
  }
  
  //===================转角以及电机控制====================//
  //加权平均法求方向控制
  g_DirectionControlWhole = 0;
  g_DirectionControlLine = 0;
  for (i = 0; i < g_CenterNum; i ++)
  {
    if (g_CenterPosition[i].y >= 0 && g_CenterPosition[i].y <= MAX_VIDEO_POINT)
    {
      g_DirectionControlLine += (int)g_CenterPosition[i].x;
      g_DirectionControlWhole += (int)g_CenterPosition[i].y * g_CenterPosition[i].x;  //注意数据不要溢出    
    }
  }
  if (g_DirectionControlLine > 0)
  {
    g_DirectionControl = g_DirectionControlWhole / g_DirectionControlLine;
  }
  //方向控制限定
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
  
  //==================向上位机发送图像=======================//
  if (g_SendPic)
  {
    // 头同步
    uart_putvec("\xfe\x01\xfe\x01\xfe", 5);
    
    int16 k;
    // 数据总长
    k = 14 + MAX_VIDEO_LINE*MAX_VIDEO_POINT + 2 + MAX_POINT_NUM*8+4 + MAX_POINT_NUM*4+2 + 5;
    uart_putchar(k % 256);uart_putchar(k / 256);
    // 序号
    k = g_SendIndex;
    uart_putchar(k % 256);uart_putchar(k / 256);
    // 行数
    k = MAX_VIDEO_LINE;
    uart_putchar(k % 256);uart_putchar(k / 256);
    // 列数
    k = MAX_VIDEO_POINT;
    uart_putchar(k % 256);uart_putchar(k / 256);
    // 图像阈值
    k = IMAGE_MIDDLE;
    uart_putchar(k % 256);uart_putchar(k / 256);
    // 边沿点数
    k = MAX_POINT_NUM;
    uart_putchar(k % 256);uart_putchar(k / 256);
    // 中心点数
    k = MAX_POINT_NUM;
    uart_putchar(k % 256);uart_putchar(k / 256);
    
    // 图像数据
    uart_putvec(&video_ori[0][0], MAX_VIDEO_LINE*MAX_VIDEO_POINT);
    k = g_DirectionControl; // 控制中心
    uart_putchar(k % 256);uart_putchar(k / 256);
    
    // 边沿点
    k = g_LeftEdgeNum; // 左边沿有效点数
    uart_putchar(k % 256);uart_putchar(k / 256);
    for(i=0; i<MAX_POINT_NUM; i++)
    {
      uart_putvec((INT8U*)&g_LeftEdge[i].x, 2);// x
      uart_putvec((INT8U*)&g_LeftEdge[i].y, 2);// y
    }
    k = g_RightEdgeNum; // 右边沿有效点数
    uart_putchar(k % 256);uart_putchar(k / 256);
    for(i=0; i<MAX_POINT_NUM; i++)
    {
      uart_putvec((INT8U*)&g_RightEdge[i].x, 2);// x
      uart_putvec((INT8U*)&g_RightEdge[i].y, 2);// y
    }
    
    // 中心点
    k = g_CenterNum; // 中心点有效个数
    uart_putchar(k % 256);uart_putchar(k / 256);
    for(i=0; i<MAX_POINT_NUM; i++)
    {
      uart_putvec((INT8U*)&g_CenterPosition[i].x, 2);// x
      uart_putvec((INT8U*)&g_CenterPosition[i].y, 2);// y
    }
    
    // 其他数据
    uart_putvec("\x15\x16\x17\x18\x19", 5);
    
    // 尾同步
    uart_putvec("\xfd\x02\xfd\x02\xfd", 5);
    
    g_SendIndex ++;
  }
  
  //======================状态转换=========================//
  
  g_VideoFlag = VIDEO_FINISH;
  //ENABLE_INTERRUPT_IRQ();   //行中断使能
  
  //标定时序
  if (g_Display % 5 == 0)
  {
    LED_1_REV();
  }
}

//计时时间
int16 g_time = 0;

//速控变量
uint8 g_speed_count;
int16 g_speed_num = 20;//每20个中断速控一次

//stand参数
float g_kgyo;
float g_b_tg;
float g_stand_p;
float g_stand_d;

//run参数
float g_speed_integral;
float g_speed_p;
float g_speed_i;
float speed;
float speed_feedback;
float speed_feedback_pre;

//turn参数
float g_direction_p;
float g_direction_d;
float g_biaslast = 0; 

//左右轮脉冲计数
int16 g_countervalue_l = 0;
int16 g_countervalue_r = 0;
float g_feedback_l = 0;
float g_feedback_r = 0;

//左右电机控制量
int16 g_motor_l,g_motor_r;

//速度给定
int16  g_speed_final;

//车状态量
CarStatus carsta;

//车控制量
CarControll carcon;

/******************************************************************************/
unsigned char IsSend = 0;
float AccZAngle = 0;



void pit0_isr(void)
{
  PIT_TFLG0|=PIT_TFLG_TIF_MASK;
 
  //计时重置
  TIME0_RESET();
  g_countervalue_r = -Get_Counter_Value(2);
  g_countervalue_l = -Get_Counter_Value(1); 
  
  //速度反馈处理
  g_feedback_l += g_countervalue_l;
  g_feedback_r += g_countervalue_r;
  
  TIME1_RESET();
  Sensor_Process();
  
  /******************************************
  以上是————传感器采集部分
  ******************************************/  
  //控制量计算
  Car_Stand(); //直立控制 
  Car_Run(); //速度控制  
  Car_Turn(); //转向控制
  
  //电机赋值
  Motor_Set(); //电机赋P波
  
  //串口调试
  if(g_SendCurve)
    uart_test();  

  g_time++;
  
} // 控制中断
 

/*
function name:  running_init
description:    init
*/
void Running_Init(void)
{
    //直立参数
    g_stand_p = 50;
    g_stand_d = 0.03;
    
    //预期中值
    Middle_Angle = 73;

    //速度参数
    g_speed_final = 300;
    g_speed_p = 1;
    g_speed_i = 0.1;
    
    //转向参数
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
      carcon.run_set = 0; //前1.5秒不给速度
  
  //分200次使速度达到设定值
  carcon.run_set += (g_speed_final - carcon.run_set)/200;

  carcon.run = (carcon.run_new - carcon.run_pre) * g_speed_count / g_speed_num + carcon.run_pre;
  
  g_speed_count ++;

  //p 起步前倾，倒地，，p不能消除稳态误差
  if(g_speed_count > g_speed_num)
  {
    speed_feedback = carcon.run_set - (g_feedback_l+g_feedback_r)/2;
    
    speed_feedback_pre = speed_feedback;
    
    g_speed_integral += speed_feedback * g_speed_i;
    
    //积分限幅！！！
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
    //控制融合
    //carcon.motor_left  = carcon.stand - carcon.run + carcon.turn ;
    //carcon.motor_right = carcon.stand - carcon.run - carcon.turn ;
    carcon.motor_left  = carcon.stand ;
    carcon.motor_right = carcon.stand ;
  
    //速度保护
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
    
    //角度保护
    //if(g_time > 2000)  //车保护角度
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
    

    //car_angle:发送占两个字节的滤波角度（carsta.car_angle）
    tempp = (int16)carsta.car_angle;
    temp = tempp;
    uart_putchar((uint8)(temp>>8));
    uart_putchar((uint8)(temp));
                 
    //angle_v：发送占两个字节的陀螺仪值（Gyro.angle_v_X）
    tempp = (int16) CarSta.Pitch;
    temp = tempp;
    uart_putchar((uint8)(temp>>8));
    uart_putchar((uint8)(temp));
    
    //angle_z：发送占两个字节的加速度计值（Acc.Zdata - Acc.middle_Z）
    tempp = (int16)( CarSta.PitchRate);
    temp = tempp;
    uart_putchar((uint8)(temp>>8));
    uart_putchar((uint8)temp);
    
    
    uart_putchar(0xAA);
}
