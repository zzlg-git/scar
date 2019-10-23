#include "BMX055.h"


//Datadef  BMX055_data;



unsigned char  BMX055_init(void)
{
  /************************************/
  /*���ٶ�����*/
  /************************************/
  uint8 ErrCount = 0;
  while(IIC_Read_Reg(IIC_BMX055_ACC_ADR, BMX055_ACC_ID) != 0xFA)   /////ȷ��оƬID
  {
    ErrCount++;
    if(ErrCount > 5)
      return 0;
  }
  if(IIC_Write_Reg(IIC_BMX055_ACC_ADR, BMX055_ACC_PMURANGE, 0x05) == 0)return 0;   ///4G      3 2G  5 4G  8 8G  
  Delay_T3_mS(10);
  if(IIC_Write_Reg(IIC_BMX055_ACC_ADR, BMX055_ACC_PMUBW, 0x0F) == 0)return 0;     ///1000HZ      
  Delay_T3_mS(10);
  if(IIC_Write_Reg(IIC_BMX055_ACC_ADR, BMX055_ACC_PMULPM, 0x00) == 0)return 0;   ///Normal MODE      
  Delay_T3_mS(10); 
  
  /************************************/
  /*����������*/
  /************************************/
  ErrCount = 0;
  while(IIC_Read_Reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_ID) != 0x0F)   /////ȷ��оƬID
  {
    ErrCount++;
    if(ErrCount > 5)
      return 0;
  }
  if(IIC_Write_Reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_RANGE, 0x01) == 0)return 0;   ///+-1000      
  Delay_T3_mS(10);
  if(IIC_Write_Reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_BW, 0x02) == 0)return 0;     ///1000HZ      
  Delay_T3_mS(10);
  if(IIC_Write_Reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_LPM, 0x00) == 0)return 0;   ///Normal MODE      
  Delay_T3_mS(10);
  if(IIC_Write_Reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_RATEHBW, 0x08) == 0)return 0;   ///��ͨ�˲� �����Բ�Ҫ      
  Delay_T3_mS(10);
  
  /************************************/
  /*����������*/
  /************************************/
  ErrCount = 0;
  IIC_Write_Reg(IIC_BMX055_MAG_ADR, BMX055_MAG_POM, 0x81);
  Delay_T3_mS(10);
  while(IIC_Read_Reg(IIC_BMX055_MAG_ADR, BMX055_MAG_ID) != 0x32)   /////ȷ��оƬID
  {
    ErrCount++;
    if(ErrCount > 5)
      return 0;
  }
  if(IIC_Write_Reg(IIC_BMX055_MAG_ADR, BMX055_MAG_DATARATE, 0x38) == 0)return 0;   ///�������30HZ       
  Delay_T3_mS(10);
  if(IIC_Write_Reg(IIC_BMX055_MAG_ADR, BMX055_MAG_INTEN, 0x00) == 0)return 0;      ///��ʹ���ж�      
  Delay_T3_mS(10);
  return 1;
}


unsigned char  BMX055_DataRead(Datadef *Q, uint8 type)
{
  uint8 datatemp[6] = {0};
  if(IIC_Read_Buff(IIC_BMX055_GYRO_ADR, BMX055_GYRO_XDATALSB, datatemp, 6) == 0)return 0;
  Q->GYROXdata = (float)((int16)((datatemp[1] << 8) | datatemp[0]));
  Q->GYROYdata = (float)((int16)((datatemp[3] << 8) | datatemp[2]));
  Q->GYROZdata = (float)((int16)((datatemp[5] << 8) | datatemp[4]));
  if(IIC_Read_Buff(IIC_BMX055_ACC_ADR, BMX055_ACC_XDATALSB, datatemp, 6) == 0)return 0;
  Q->ACCXdata = (float)((int16)((datatemp[1] << 8) | datatemp[0]) >> 4);
  Q->ACCYdata = (float)((int16)((datatemp[3] << 8) | datatemp[2]) >> 4);
  Q->ACCZdata = (float)((int16)((datatemp[5] << 8) | datatemp[4]) >> 4);
  /************************************/
  /*���������ݶ�ȡ*/
  /************************************/
  if(type)
  {
    if(IIC_Read_Buff(IIC_BMX055_MAG_ADR, BMX055_MAG_XDATALSB, datatemp, 6) == 0)return 0;
    Q->MAGXdata = (float)((int16)((datatemp[1] << 8) | datatemp[0]) >> 3);
    Q->MAGYdata = (float)((int16)((datatemp[3] << 8) | datatemp[2]) >> 3);
    Q->MAGZdata = (float)((int16)((datatemp[5] << 8) | datatemp[4]) >> 1);
  }
  return 1;
}


unsigned char  BMX055_DataRead0(Datadef *Q)
{
  uint8 datatemp[4] = {0};
  if(IIC_Read_Buff(IIC_BMX055_GYRO_ADR, BMX055_GYRO_XDATALSB, datatemp, 4) == 0)return 0;
  Q->GYROXdata = (float)((int16)((datatemp[1] << 8) | datatemp[0]));
  Q->GYROYdata = (float)((int16)((datatemp[3] << 8) | datatemp[2]));

  return 1;
}