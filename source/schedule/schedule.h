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
#define MID_SERVO_VALUE	    1520     //�����ֵ���Լ�����
#define MAX_SERVO_VALUE     1700     //���ת�����ֵ���Լ�����
#define MIN_SERVO_VALUE     1340     //���ת����Сֵ���Լ�����

/*
* ����ͷ����
*/
#define   START_VIDEO_LINE	        105          	//ͼ����ʼ��
#define   MAX_VIDEO_POINT           162             //ÿ�вɼ��ĵ���
#define   MID_VIDEO_POINT           81              //ͼ�����ĵ�
#define   MAX_VIDEO_LINE	        60              //ÿ��ʵ���õ�������
#define   VIDEO_PORT                GPIOC_PDIR      //����ͷ�ź�

#define   SetBit(reg,bit)		    reg |= (1 << bit)
#define   ClrBit(reg,bit)		    reg &= ~(1 << bit)


/*
* ͼ��ɼ�
*/
typedef enum        //ͼ��ɼ�״̬����״̬������״ֵ̬��Ϊö����
{
    VIDEO_WAIT,
    VIDEO_ACQUISITION,
    VIDEO_SEARCH,
    VIDEO_FINISH,
}Video_state;

typedef struct      //ͼ�����ݽṹ  �����Ͻ�Ϊ����ԭ�� ����Ϊx������Ϊy
{
    int16 x;
    int16 y;
}Int16_point;

/*********ͼ����********/
#define   MAX_POINT_NUM                 80                  //���غ�����������
#define   IMAGE_MIDDLE                  159                 //ͼ����ֵ�������Լ���ͼ���ʵ�����
#define   WIDTH                         60                  //��һ�а׿���Ҫ�� ����ǰհ�Լ�������ͼ������ʵ�����
#define   INIT_DATA			            250                 //��Ч����

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
void vsync_isr(void);					//���ж�
void href_isr(void);					//���ж�



/*----------------------- D E C L A R A T I O N ------------------------------*/
// Function prototypes
void ScheduleInit(void);
void Running(void);

//light10th
void StartLightJudge(void);		//�����ж�


#endif /* __SCHEDULE_H__ */




