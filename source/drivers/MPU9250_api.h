#ifndef __MPU9250_API_H__
#define __MPU9250_API_H__

#include "MPU9250.h"

extern float q[4]; /* ???*/
extern float gyrof[3];  /* X/Y/Z ???*/
extern float accelf[3]; /* X/Y/Z ??? */
extern float yprf[3];   /* yaw/pitch/roll*/
extern float mag[3];


//��ʼ��MPU9250�����ҿ���DMP�������ж�ģʽΪ���ٶ��ж�
extern void Init_MPU9250_With_DMP(void);

//��λFIFO
extern void MPU9250_resetFIFO(void);

//��ȡFIFO�ļ���
extern uint16_t MPU9250_getFIFOCount(void);

//��ȡyaw/pitch/roll
extern void getyawpitchroll(void);

//��ȡ��Ԫ��
extern void getquaternion(void);

//��ȡ X/Y/Z ������ֵ
extern void getmag(void);

//��ȡDMP FIFO
extern void readdmp(void);

//��ȡ���ٶȡ����ٶ�
extern void getAcc_gyro(void);

//�ж�MPU9250�Ƿ��Ѿ��������ж� 1�������� 0��δ����
extern uint8_t MPU9250_Interrupted(void);

//����MPU9250
extern void MPU9250_Sleep(void);

//����MPU9250
extern void MPU9250_WakeUp(void);

#endif

//------------------End of File----------------------------
