/*
 * File:	schedule.h
 * Purpose:	schedule
 *
 * Notes:		
 */

#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

/******************************************************************************/
typedef uint8   INT8U;
typedef int8    INT8S;
typedef uint16  INT16U;
typedef int16   INT16S;
typedef uint32  INT32U;
typedef int32   INT32S;
typedef uint64  INT64U;
typedef int64   INT64S;

/*----------------------- M A C R O S ----------------------------------------*/
#define MID_SERVO_VALUE	    1520     //舵机中值，自己调节
#define MAX_SERVO_VALUE     1700     //舵机转角最大值，自己调节
#define MIN_SERVO_VALUE     1340     //舵机转角最小值，自己调节

/*
* 摄像头参数
*/
#define   START_VIDEO_LINE	        105          	//图像起始行
#define   MAX_VIDEO_POINT           162             //每行采集的点数
#define   MID_VIDEO_POINT           81              //图像中心点
#define   MAX_VIDEO_LINE	        60              //每场实际用到的行数
#define   VIDEO_PORT                GPIOC_PDIR      //摄像头信号

#define   SetBit(reg,bit)		    reg |= (1 << bit)
#define   ClrBit(reg,bit)		    reg &= ~(1 << bit)


/*
* 图像采集
*/
typedef enum        //图像采集状态机的状态变量的状态值，为枚举形
{
    VIDEO_WAIT,
    VIDEO_ACQUISITION,
    VIDEO_SEARCH,
    VIDEO_FINISH,
}Video_state;

typedef struct      //图像数据结构  以左上角为坐标原点 向下为x，向右为y
{
    int16 x;
    int16 y;
}Int16_point;

/*********图像处理********/
#define   MAX_POINT_NUM                 80                  //边沿和中心最大个数
#define   IMAGE_MIDDLE                  159                 //图像阈值，根据自己的图像适当调整
#define   WIDTH                         60                  //第一行白块宽度要求 根据前瞻以及看到的图像情况适当调整
#define   INIT_DATA			            250                 //无效数据

/*----------------------- D E C L A R A T I O N ------------------------------*/
extern Video_state  g_VideoFlag;

void ScheduleInit(void);
void irq_isr(void);
void Search(void);
/******************************************************************************/



/******************************************************************************/
  typedef struct carstatus
{
    float car_angle;
    int bias;
    int32 Car_speed_set;
}CarStatus,*pCarStatus;

  typedef struct carcontroll
{
    float stand;
    float run;
    float turn;
    float run_set;
    float run_pre;
    float run_new;
    float motor_right;
    float motor_left;
}CarControll,*pCarControll;
/******************************************************************************/

extern int16 g_time;
extern CarStatus carsta;
extern int16 g_speed_final;
extern int16 g_countervalue_l;
extern int16 g_countervalue_r;
extern int g_sendflag;
extern int16 g_speed_num;

/******************************************************************************/


extern int16 g_countervalue_l;
extern int16 g_countervalue_r;

void pit0_isr(void);
void Running_Init(void);
void Car_Stand(void);
void Car_Run(void);
void Car_Turn(void);
void Motor_Set(void);
void uart_test(void);
void vsync_isr(void);					//场中断
void href_isr(void);					//行中断



/*----------------------- D E C L A R A T I O N ------------------------------*/
// Function prototypes
void ScheduleInit(void);
void Running(void);

//light10th
void StartLightJudge(void);		//灯塔判定


#endif /* __SCHEDULE_H__ */




