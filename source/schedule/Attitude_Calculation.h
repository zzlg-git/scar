#ifndef __ATTITUDE_CAUCULATION_H__
#define __ATTITUDE_CAUCULATION_H__


//#define  PI            3.1415926f
#define  PERIODHZ      (float)250       /////����Ƶ��
#define  PERIODS       (float)0.004          ////��������


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
  float Pitch;  //������
  float Yaw;    //ƫ����
  float Roll;   //������
}EulerAngleTypedef;


typedef struct{
  float Xdata;
  float Ydata;
  float Zdata;
}AttitudeDatatypedef;

typedef struct{ 
  float Pitch;  //������
  float Yaw;    //ƫ����
  float Roll;   //������
  float PitchRate;  //������
  float YawRate;    //ƫ����
  float RollRate;   //������
  float test;  //������
}CarAttitudeTypedef;



extern AttitudeDatatypedef GyroOffset;


extern int16 CarSetAngle;


extern CarAttitudeTypedef    CarSta;   //ŷ����

extern QuaternionTypedef    Quaternion;   //��Ԫ��
extern EulerAngleTypedef    EulerAngle;   //ŷ����

//extern QuaternionTypedef    TestQuaternion;   //��Ԫ��
//extern EulerAngleTypedef    TestEulerAngle;   //ŷ����

extern QuaternionTypedef    AxisAngle;    //���
extern EulerAngleTypedef    EulerAngleRate;//��ǰŷ�����ٶ�

extern QuaternionTypedef    MeaQuaternion;//���ٶ����
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