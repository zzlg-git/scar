/*
 * File:	sensoroperation.h
 * Purpose:	sensoroperation 
 *
 * Notes:		
 */

#ifndef __SENSOROPERATION_H__
#define __SENSOROPERATION_H__

/******************************************************************************/
#define K_GyroInitNUM  1000
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
    float angle_v;
}Gyo_data,*pGyo_data;

/******************************************************************************/

// Function prototypes
void Sensor_Init(void);
void Gyro_Init(void);
void Acc_AD_Sample(void);
void Gyo_AD_Sample(void);
void Sensor_Process(void);

/******************************************************************************/

//extern
extern Acc_data acc;
extern Gyo_data gyo;
extern float angle_v;
extern Acc_data acc;
extern Gyo_data gyo1,gyo2;

/******************************************************************************/
#endif /* __SENSOROPERATION_H__ */