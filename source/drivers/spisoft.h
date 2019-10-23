#ifndef __SPISOFT_H__
#define __SPISOFT_H__

/*--------------- I N C L U D E S ------------------------------------*/

/*-------------- - M A C R O S ----------------------------------------*/

#define SPI2_SCLK_PIN   (21)
#define SPI2_SCLK_DOR   GPIOB_PDOR
#define SPI2_SCLK_DDR   GPIOB_PDDR
#define SPI2_SCLK_PCR   PORTB_PCR21 //output

#define SPI2_SDO_PIN    (22)
#define SPI2_SDO_DOR    GPIOB_PDOR
#define SPI2_SDO_DDR    GPIOB_PDDR
#define SPI2_SDO_PCR    PORTB_PCR22 //output

#define SPI2_SDI_PIN    (23)
#define SPI2_SDI_DIR    GPIOB_PDIR
#define SPI2_SDI_DDR    GPIOB_PDDR
#define SPI2_SDI_PCR    PORTB_PCR23 //input

#define FXAS21002_CS_PIN     (20)
#define FXAS21002_CS_DOR     GPIOB_PDOR
#define FXAS21002_CS_DDR     GPIOB_PDDR
#define FXAS21002_CS_PCR     PORTB_PCR20 //output



#define SPI2_SCLK_HIGH()        SET_BIT(SPI2_SCLK_DOR, SPI2_SCLK_PIN)
#define SPI2_SCLK_LOW()         CLR_BIT(SPI2_SCLK_DOR, SPI2_SCLK_PIN)

#define SPI2_SDI_STATUS()       BIT_IS_HIGH(SPI2_SDI_DIR, SPI2_SDI_PIN)

#define SPI2_SDO_HIGH()         SET_BIT(SPI2_SDO_DOR, SPI2_SDO_PIN)
#define SPI2_SDO_LOW()          CLR_BIT(SPI2_SDO_DOR, SPI2_SDO_PIN)

#define MPU9250_CS_HIGH()          SET_BIT(FXAS21002_CS_DOR, FXAS21002_CS_PIN)
#define MPU9250_CS_LOW()           CLR_BIT(FXAS21002_CS_DOR, FXAS21002_CS_PIN)

/*-------------- F U N C T I O N S ------------------------------------*/
//====================spi2====================//
void init_spisoft(void);
void spi_send_byte(unsigned char dat);
unsigned char spi_receive_byte(void);
#endif

