#include "iicsoft.h"


void IIC_DELAY(uint8 m)/////延时信号
{
  uint8 i = 0;    
  while(m--)
  {
    i = IICDelay;
    while(i--);
  }
}


void IIC_start(void)////起始信号
{
  IIC_SDA_OUT();
  IIC_DELAY(2);
  IIC_SDA_H; 
  IIC_SCL_H;
  IIC_DELAY(2);
  IIC_SDA_L;
  IIC_DELAY(2); 
  IIC_SCL_L;
  IIC_DELAY(2);
}

void IIC_stop(void)/////停止信号
{ 
    IIC_SDA_H;
    IIC_SCL_L;
    IIC_DELAY(2);
    IIC_SCL_H; 
    IIC_DELAY(2);
    IIC_SDA_H;
    IIC_DELAY(2);
    IIC_SCL_H;
}

unsigned char IIC_waitack(void)
{
  uint8 ErrTime=0;
  IIC_SCL_L;
  IIC_DELAY(1);
  IIC_SDA_H;
  IIC_DELAY(1);
  IIC_SDA_IN();      //SDA设置为输入  
  IIC_DELAY(1);  
  IIC_SCL_H;
  IIC_DELAY(1);	 
  while(IIC_DATA)
  {
     ErrTime++;
    if(ErrTime > 250)
    {
	IIC_stop();
	return 1;
    }
  }
  IIC_SCL_L;//时钟输出0 
  IIC_SDA_OUT();
  IIC_DELAY(1);
  return 0;  
}

void IIC_ack(void)////应答信号
{	
  IIC_SCL_L;
  IIC_DELAY(1);
  IIC_SDA_L;
  IIC_DELAY(1);
  IIC_SCL_H;
  IIC_DELAY(1);
  IIC_SCL_L;
  IIC_DELAY(1);
}

void IIC_noack(void)////无应答信号
{
  IIC_SCL_L;
  IIC_DELAY(1);
  IIC_SDA_H;
  IIC_DELAY(1);
  IIC_SCL_H;
  IIC_DELAY(1);
  IIC_SCL_L;
  IIC_DELAY(1);
}

void IIC_sendbyte(uint8 data)
{
  uint8 i = 8;
  while(i--)
  {
    IIC_SCL_L;
    if(data & 0x80)
      IIC_SDA_H; 
    else 
      IIC_SDA_L;   
    data <<= 1;
    IIC_DELAY(1);
    IIC_SCL_H;
    IIC_DELAY(1);
  }
}

uint8 IIC_readbyte(void)
{
  uint8 i=8;
  uint8 ReceiveByte=0;
  IIC_SDA_H;	
  IIC_SDA_IN();	
  while(i--)
  {
    ReceiveByte <<= 1;      
    IIC_SCL_L;
    IIC_DELAY(1);
    IIC_SCL_H;
    IIC_DELAY(1);
    if(IIC_DATA)
    {
      ReceiveByte |= 0x01;
    }  
  }
  IIC_SDA_OUT();
  IIC_SCL_L;
  return ReceiveByte; 
}

///////错误返回false
unsigned char  IIC_Write_Reg(uint8 addr, uint8 offset, uint8 data)
{
  IIC_start();
  IIC_sendbyte(addr << 1);
  if(IIC_waitack())return  0;
  IIC_sendbyte(offset);
  if(IIC_waitack())return 0;
  IIC_sendbyte(data);
  if(IIC_waitack())return 0;
  IIC_stop();
  return 1;
}

///////错误数据返回false
uint8 IIC_Read_Reg(uint8 addr, uint8 offset)
{
  uint8 data = 0;
  IIC_start();
  IIC_sendbyte(addr << 1);
  if(IIC_waitack())return 0;
  IIC_sendbyte(offset);
  if(IIC_waitack())return 0;
  IIC_start();
  IIC_sendbyte((addr << 1) + 1);
  if(IIC_waitack())return 0;
  data = IIC_readbyte();
  IIC_noack();
  IIC_stop();
  return data;
}

unsigned char  IIC_Read_Buff(uint8 addr, uint8 offset, uint8* buff, uint8 size)
{
  uint8 i = 0;
  IIC_start();
  IIC_sendbyte(addr << 1);
  if(IIC_waitack())return 0;
  IIC_sendbyte(offset);
  if(IIC_waitack())return 0;
  IIC_start();
  IIC_sendbyte((addr << 1) + 1);
  if(IIC_waitack())return 0;
  while(i < size)
  {
    *(buff + i) = IIC_readbyte();
    i++;  
    if(i == size)
    {
      IIC_noack();
      IIC_stop();
    }
    else
      IIC_ack();
  }
  return 1;
}


void IIC_init(void)
{

  IICCLOCKON;
  IIC_PORT->PCR[IIC_SDApin] = (0x01 << PORT_PCR_MUX_SHIFT) | (0x03 << PORT_PCR_PS_SHIFT) | (0x01 << PORT_PCR_ODE_SHIFT);
  IIC_PORT->PCR[IIC_SCLpin] =  (0x01 << PORT_PCR_MUX_SHIFT) | (0x03 << PORT_PCR_PS_SHIFT) | (0x01 << PORT_PCR_ODE_SHIFT);
  IIC_GPIO->PDDR |= (1 << IIC_SDApin);
  IIC_GPIO->PDDR |= (1 << IIC_SCLpin);
  IIC_SDA_H;
  IIC_SCL_H;

}