#ifndef __ATTITUDE_CAUCULATION_H__
#define __ATTITUDE_CAUCULATION_H__


//#define  PI            3.1415926f
#define  PERIODHZ      (float)250       /////采样频率
#define  PERIODS       (float)0.004          ////采样周期


#define true    1
#define false   0



#define  XA   (Acc.Xdata)
#define  YA   (-Acc.Zdata) 
#define  ZA   (-Acc.Ydata )

#define  XG   ( -Gyro.Xdata)
#define  YG   ( Gyro.Zdata)
#define  ZG   ( Gyro.Ydata)


//#define  XA   (-Acc.Xdata)
//#define  YA   ( Acc.Zdata)  
//#define  ZA   (-Acc.Ydata)
//
//#define  XG   ( Gyro.Xdata)
//#define  YG   (-Gyro.Zdata)
//#define  ZG   ( Gyro.Ydata)

#define  ATTITUDE_COMPENSATE_LIMIT   ((float)1 / 180 * PI / PERIODHZ)


typedef struct{
  float W;
  float X;
  float Y;
  float Z;
}QuaternionTypedef;

typedef struct{ 
  float Pitch;  //俯仰角
  float Yaw;    //偏航角
  float Roll;   //翻滚角
}EulerAngleTypedef;


typedef struct{
  float Xdata;
  float Ydata;
  float Zdata;
}AttitudeDatatypedef;

typedef struct{ 
  float Pitch;  //俯仰角
  float Yaw;    //偏航角
  float Roll;   //翻滚角
  float PitchRate;  //俯仰角
  float YawRate;    //偏航角
  float RollRate;   //翻滚角
  float test;  //俯仰角
}CarAttitudeTypedef;



extern AttitudeDatatypedef GyroOffset;


extern int16 CarSetAngle;


extern CarAttitudeTypedef    CarSta;   //欧拉角

extern QuaternionTypedef    Quaternion;   //四元数
extern EulerAngleTypedef    EulerAngle;   //欧拉角

//extern QuaternionTypedef    TestQuaternion;   //四元数
//extern EulerAngleTypedef    TestEulerAngle;   //欧拉角

extern QuaternionTypedef    AxisAngle;    //轴角
extern EulerAngleTypedef    EulerAngleRate;//当前欧拉角速度

extern QuaternionTypedef    MeaQuaternion;//加速度求得
extern EulerAngleTypedef    MeaEulerAngle;
extern QuaternionTypedef    MeaAxisAngle;

extern QuaternionTypedef    ErrQuaternion;
extern EulerAngleTypedef    ErrEulerAngle;
extern QuaternionTypedef    ErrAxisAngle;
extern AttitudeDatatypedef         Acc;
extern AttitudeDatatypedef         Gyro;


extern void Quaternion_init();

extern void Attitude_UpdateGyro(void);

extern void Attitude_UpdateAcc(void);
#endif