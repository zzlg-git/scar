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
    //值初始化
    acc.x_sample = 0;
    acc.y_sample = 0;
    acc.z_sample = 0;
    acc.angle_z = 0;
    
    gyo1.g_sample = 0;
    gyo1.angle_v = 0;
    
    gyo2.g_sample = 0;
    gyo2.angle_v = 0;  
    
    Gyro_Init();
    
}


void Gyro_Init(void)
{
    int16 i=0;
    int32 Gyro1InitSum=0;
    int32 Gyro2InitSum=0;
    
    //Delay_T3_mS(500);
    for(i=0;i<K_GyroInitNUM+500;i++)
    {
        
        Delay_T3_uS(100);
        //Sensor_Process();
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
function name:  sensor_process
parameters:     none
author:         How
date:           2012-6-29
description:    
*/
void Sensor_Process(void)
{
        
  static uint8 IsAttitudeinit = false;
  
  BMX055_Sample();
      
  if(IsAttitudeinit == false)
  {
    Quaternion_init();                    ////姿态解算初始化        
    IsAttitudeinit = true;
  }
  else
  {

    Attitude_UpdateGyro();                /////Gyro快速更新
    
    Attitude_UpdateAcc();                 /////ACC深度融合更新
    
    
    CarSta.Pitch = -EulerAngle.Roll / PI * 180;            ////转为角度   俯仰角
    CarSta.Roll = EulerAngle.Pitch / PI * 180;             ////翻滚角
    CarSta.Yaw = EulerAngle.Yaw / PI * 180;                ////偏航角
    CarSta.PitchRate = -EulerAngleRate.Roll / PI * 180;   ////俯仰角速度
    CarSta.YawRate = EulerAngleRate.Yaw / PI * 180;       ////偏航角速度
    
    static float testpitchangle=0;
    static float testpitchangle1=0;
    static float testpitchangle2=0;
    testpitchangle+= Gyro.Ydata;
    testpitchangle1+=Gyro.Xdata;
    testpitchangle2+=Gyro.Zdata;

  }
    
}

float bmx055test = 0;
void BMX055_Sample(void)
{
  
  //  MAGWaitCount++;
  //  if(MAGWaitCount < 15)
  //    BMX055_DataRead(&BMX055_data, 0);
  //  else
  //  {
  //    BMX055_DataRead(&BMX055_data, 1);     //////每30ms读取一次磁力计
  //    MAGWaitCount = 0;
  //  }

    BMX055_DataRead(&BMX055_data, 0); 
 

  //////每30ms读取一次磁力计

  BMX055_data.GYROXdata = (BMX055_data.GYROXdata - GyroOffset.Xdata) * 0.030517578;   
  BMX055_data.GYROYdata = (BMX055_data.GYROYdata - GyroOffset.Ydata) * 0.030517578;
  BMX055_data.GYROZdata = (BMX055_data.GYROZdata - GyroOffset.Zdata) * 0.030517578;
  ///////1000 / 32768     //////BMX055本身零飘几乎可以忽略不计，但是安全起见还是矫正一下
  BMX055_data.ACCXdata *= 0.001953125;    ///////4 / 2048
  BMX055_data.ACCYdata *= 0.001953125;
  BMX055_data.ACCZdata *= 0.001953125;
  
  
  Acc.Xdata = BMX055_data.ACCXdata;
  Acc.Ydata = BMX055_data.ACCYdata;
  Acc.Zdata = BMX055_data.ACCZdata;
  
  bmx055test =  Acc.Xdata *  Acc.Xdata + Acc.Ydata * Acc.Ydata + Acc.Zdata  * Acc.Zdata;
    
  Gyro.Xdata = BMX055_data.GYROXdata;
  Gyro.Ydata = BMX055_data.GYROYdata;
  Gyro.Zdata = BMX055_data.GYROZdata;
  
  
  //  Gyro.Xdata = (MPU9250_Date.GYROXdata - MPU9250_Date.GYROXoffset) * 0.030518044;
  //  Gyro.Ydata = (MPU9250_Date.GYROYdata - MPU9250_Date.GYROYoffset) * 0.030518044;
  //  Gyro.Zdata = (MPU9250_Date.GYROZdata - MPU9250_Date.GYROZoffset) * 0.030518044;
  //  
  //  Acc.Xdata = MPU9250_Date.ACCXdata * 0.000122072;
  //  Acc.Ydata = MPU9250_Date.ACCYdata * 0.000122072;
  //  Acc.Zdata = MPU9250_Date.ACCZdata * 0.000122072;
  
  

  
}
