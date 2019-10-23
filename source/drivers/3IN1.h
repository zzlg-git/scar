/*
 * File:	3IN1.h
 * Purpose:
 *
 * Notes:		
 */

#ifndef __3IN1_H__
#define __3IN1_H__

//================================宏定义======================================//
#define PressStill              1  //1为区分持续按键，0为不区分持续按键
   
#define XLevelL         0x00
#define XLevelH         0x10
#define XLevel          ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column      128
#define Max_Row         64
#define Brightness      0xCF

#define X_WIDTH         128
#define Y_WIDTH         64

#define SIZE_0608		0
#define SIZE_0816		1
#define	FONT_SIZE_L		SIZE_0816
#define FONT_SIZE_S		SIZE_0608
#define device_add0		0x78   
#define reg_add0		0x00  

//===================================变量声明=================================//
extern uint8 EEPROM_high_0816;
extern uint8 EEPROM_low_0816;
extern uint8 EEPROM_high_0608;
extern uint8 EEPROM_low_0608;
extern uint16 EEPROM_middle_0608;
extern uint16 EEPROM_middle_0816;

//===================================函数声明=================================//
extern void Init_OLED();
extern void fill_picture(unsigned char fill_Data);
extern void Init_CH455();
extern uint8 CH455_Key();
extern void OLED_Reset(unsigned char DATA);     //OLED 通过CH455重启  
extern void OLED_RAM_Refresh_add(void);
extern void OLED_RAM_Refresh_add_Line(uint8 line);

extern void Keyboard_display_str_1ST(const unsigned char * str,unsigned char size);
extern void Keyboard_display_str_2ND(const unsigned char * str,unsigned char size);
extern void Keyboard_display_str_3TH(const unsigned char * str,unsigned char size);
extern void Keyboard_display_str_4TH(const unsigned char * str,unsigned char size);
extern void Keyboard_display_str_MT(const unsigned char * str,unsigned char size);


//extern void OLED_RAM_Fill_add(unsigned char dot);
extern void OLED_RAM_Fill_add_UP(unsigned char dot);
extern void OLED_RAM_Fill_add_DOWN(unsigned char dot);
extern void OLED_RAM_Fill_add_UL(unsigned char dot);
extern void OLED_RAM_Fill_add_UR(unsigned char dot);
extern void OLED_RAM_Fill_add_MUL(unsigned char dot);
extern void OLED_RAM_Fill_add_MUR(unsigned char dot);
extern void OLED_RAM_Fill_add_MDL(unsigned char dot);
extern void OLED_RAM_Fill_add_MDR(unsigned char dot);
extern void OLED_RAM_Fill_add_DL(unsigned char dot);
extern void OLED_RAM_Fill_add_DR(unsigned char dot);



void OLED_RAM_Refresh_add(void);
void OLED_RAM_Refresh_add_UP(void);
void OLED_RAM_Refresh_add_DOWN(void);
void OLED_RAM_Refresh_add_UL(void);
void OLED_RAM_Refresh_add_UR(void);
void OLED_RAM_Refresh_add_MUL(void);
void OLED_RAM_Refresh_add_MUR(void);
void OLED_RAM_Refresh_add_MDL(void);
void OLED_RAM_Refresh_add_MDR(void);
void OLED_RAM_Refresh_add_DL(void);
void OLED_RAM_Refresh_add_DR(void);

extern void OLED_RAM_DrawPoint_add(unsigned char x, unsigned char y, unsigned char t);
extern void OLED_RAM_ShowChar0608_add(unsigned char x, unsigned char y,
					unsigned char chr,
					unsigned char mode);
extern void OLED_RAM_ShowChar0816_add(unsigned char x, unsigned char y,
						unsigned char chr,
						unsigned char mode);
extern void OLED_RAM_Fill_add(unsigned char dot);
extern void OLED_RAM_Fill_add_ClearLine(uint8 line);
extern void OLED_RAM_disp_string_add(unsigned char x,
					unsigned char y,
					const unsigned char * str,
					unsigned char size,
					unsigned char mode);
extern void OLED_RAM_disp_num_add(unsigned char x,
					unsigned char y, 
					long  long num,
					unsigned char size,
					unsigned char mode);

extern void OLED_RAM_disp_num_test(unsigned char line,
                                        const unsigned char * str1,
                                        long num1,
                                        const unsigned char * str2,
                                        long num2,
					unsigned char size,
					unsigned char mode,
                                        int choice);
void OLED_RAM_disp_num_five(unsigned char line,
                                        int num1,
                                        int num2,
                                        unsigned char num3,
                                        unsigned char num4,
                                        unsigned char num5,
					unsigned char size,
					unsigned char mode,
                                        unsigned char choice);
#endif
/* __3IN1_H__ */