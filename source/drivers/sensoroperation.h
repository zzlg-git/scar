/*
 * File:	sensoroperation.h
 * Purpose:	sensoroperation 
 *
 * Notes:		
 */

#ifndef __SENSOROPERATION_H__
#define __SENSOROPERATION_H__

/******************************************************************************/
#define K_GyroInitNUM  10
//struct
typedef struct acc_data
{
    int32 x_sample;    //x轴采样值
    int32 y_sample;    //y轴采样值
    int32 z_sample;    //z轴采样值
    
    int32 middle_x;
    int32 middle_y;
    int32 middle_z;
    
    float angle_z;
}Acc_data,*pAcc_data;

typedef struct gyo_data
{
    int32 g_sample;   //角速度采样值
    int32 middle_g;
    int32 angle_v;
}Gyo_data,*pGyo_data;

/******************************************************************************/

// Function prototypes
void Sensor_Init(void);
void Gyro_Init(void);
void Sensor_Process(void);
void BMX055_Sample(void);

void Kalman_Filter_angle(float angle_m,float gyro_m);

/******************************************************************************/

//extern
extern Acc_data acc;
extern Gyo_data gyo;
extern float angle_v;
extern Acc_data acc;
extern Gyo_data gyo1,gyo2;
extern float bmx055test;
/*****************************数组函数***********************************************/
void update_queue(int16 *str,int length,int16 new_data);
void update_int16queue(int16 *str,int length,int16 new_data);
void update_uint16queue(uint16 *str,int length,uint16 new_data);
void init_int16queue(int16 *str,int length);
void sort_int16queue(int16 *str,int length);
void sort_uint16queue(uint16 *str,int length);

/**********************************图像矫正********************************************/






#endif /* __SENSOROPERATION_H__ */