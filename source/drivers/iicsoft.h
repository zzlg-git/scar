//#ifndef  __IICSOFT_H__
//#define  __IICSOFT_H__

#include "common.h"


/*���I2C��������*/
#define IICDelay      15        ///////������Ӳ����·��ʹ����Ӧ��I2C��ͨѶ�����õľ���С     
/**************************************/
/*  K66 210Mʱ��ֵ��Ϊ3-15    ���150us��ɲɼ�
*/
/*************************************/
#define IICCLOCKON     (SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK)
#define IIC_PORT       PORTB_BASE_PTR
#define IIC_GPIO       PTB_BASE_PTR

#define IIC_SCLpin     21
#define IIC_SDApin     20
#define IIC_SDA_OUT()  {(IIC_GPIO->PDDR |= (1 << IIC_SDApin));}
#define IIC_SDA_IN()   {(IIC_GPIO->PDDR &= ~(1 << IIC_SDApin));}
#define IIC_DATA       ((IIC_GPIO->PDIR >> IIC_SDApin) & 0x01)
#define IIC_SDA_H      (IIC_GPIO->PDOR |= (1 << IIC_SDApin))
#define IIC_SCL_H      (IIC_GPIO->PDOR |= (1 << IIC_SCLpin))
#define IIC_SDA_L      (IIC_GPIO->PDOR &= ~(1 << IIC_SDApin))
#define IIC_SCL_L      (IIC_GPIO->PDOR &= ~(1 << IIC_SCLpin))



uint8 IIC_Read_Reg(uint8 addr, uint8 offset);
unsigned char  IIC_Write_Reg(uint8 addr, uint8 offset, uint8 data);
unsigned char IIC_Read_Buff(uint8 addr, uint8 offset, uint8* buff, uint8 size);
void IIC_init(void);
//#endif