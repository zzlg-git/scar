#include "common.h"
#include "spisoft.h"

//====================spispft====================//
void init_spisoft(void)
{
    SPI2_SCLK_PCR = (0 | PORT_PCR_MUX(1));
    SPI2_SDI_PCR = (0 | PORT_PCR_MUX(1));
    SPI2_SDO_PCR = (0 | PORT_PCR_MUX(1));
    FXAS21002_CS_PCR = (0 | PORT_PCR_MUX(1));

    
    SET_BIT(SPI2_SCLK_DDR, SPI2_SCLK_PIN);
    CLR_BIT(SPI2_SDI_DDR, SPI2_SDI_PIN);
    SET_BIT(SPI2_SDO_DDR, SPI2_SDO_PIN);
    SET_BIT(FXAS21002_CS_DDR, FXAS21002_CS_PIN);

    
    MPU9250_CS_HIGH();
    SPI2_SCLK_LOW();
    SPI2_SDO_LOW();
}

void spi_send_byte(unsigned char dat)
{
    int cnt ;

    for(cnt = 0; cnt < 8; cnt ++)
    {
        if(dat & 0x80)
        {
            SPI2_SDO_HIGH();
            Delay_T3_nS(500);
        }
        else
        {
            SPI2_SDO_LOW();
            Delay_T3_nS(500);
        }
        SPI2_SCLK_HIGH();               //sclk = 1
        Delay_T3_nS(500)
        dat<<=1;
        SPI2_SCLK_LOW();                //sclk = 0
    }
}

unsigned char spi_receive_byte(void)
{
    unsigned char dat = 0, i;
    
    for(i = 0; i < 8; i++)
    {
        dat<<=1;

        SPI2_SCLK_HIGH();
        Delay_T3_nS(500)
        dat += SPI2_SDI_STATUS();

        SPI2_SCLK_LOW();
        Delay_T3_nS(500)
    }
    return dat;
}