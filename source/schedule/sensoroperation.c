/*
 * File:	sensoroperation.c
 * Purpose:	sensor operates
 *
 * Notes:		
 */
#include "common.h"

Acc_data acc;
Gyo_data gyo1;
Gyo_data gyo2;

/******************************************************************************/
//传感器值初始化
void Sensor_Init(void)
{
    //初始化中值
    //acc
    acc.middle_z=3000;//3050
//    //gyo1
//    gyo1.middle_g=1910;
//    //gyo2
//    gyo2.middle_g=2115;
    
    //值初始化
    acc.x_sample = 0;
    acc.y_sample = 0;
    acc.z_sample = 0;
    acc.angle_z = 0;
    
    gyo1.g_sample = 0;
    gyo1.angle_v = 0;
    
    gyo2.g_sample = 0;
    gyo2.angle_v = 0;  
    
   // Gyro_Init();
    
}


void Gyro_Init(void)
{
    int16 i=0;
    int32 Gyro1InitSum=0;
    int32 Gyro2InitSum=0;
    
    Delay_T3_mS(500);
    for(i=0;i<K_GyroInitNUM+500;i++)
    {
        
        Delay_T3_uS(100);
        Sensor_Process();
        if(i>499)
        {
            Gyro1InitSum += gyo1.g_sample;
            Gyro2InitSum += gyo2.g_sample;
         }
    }
    gyo1.middle_g = Gyro1InitSum / K_GyroInitNUM;
    gyo2.middle_g = Gyro2InitSum / K_GyroInitNUM; 
}

/*
function name:  acc_ad_sample
parameters:     none
author:         Xian Lee
date:           2012-6-27
description:    collect accelerator ad values
*/
void Acc_AD_Sample(void)
{
    int16 i;
    int32 sum_z=0;
    
    for(i=0;i<2;i++)
    {
        sum_z += Get_GYO_Channel(2);
    }
   
    acc.z_sample = sum_z >> 4;
    
    acc.angle_z = acc.z_sample - acc.middle_z;
    
}


/*
function name:  gyo_ad_sample
parameters:     none
author:         Xian Lee
date:           2012-6-27
description:    collect gyro ad values
*/
void Gyo_AD_Sample(void)
{
    gyo1.angle_v = gyo1.middle_g - FXAS21002_GetResult_spisoft(FXAS21002_OUTY_MSB);//转向用
    gyo2.angle_v = gyo2.middle_g - FXAS21002_GetResult_spisoft(FXAS21002_OUTX_MSB);//直立用;
}


/*
function name:  sensor_process
parameters:     none
author:         How
date:           2012-6-29
description:    
*/
void Sensor_Process(void)
{
      g_countervalue_r = Get_Counter_Value(1);
      g_countervalue_l = Get_Counter_Value(2);
      
      Acc_AD_Sample();
      Gyo_AD_Sample();
}




