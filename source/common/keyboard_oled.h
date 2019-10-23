/*
 * File:	keyboard.h
 * Purpose:	keyboard ZLG7290
 *
 * Notes:		
 */

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
//====================================================14th==========================================//


//=====================================meetcar============================//
typedef struct       //终点会车准备
{
  INT16U Break_length ;
  INT16U Break_Maxlength ;
}Break_Dis;

extern Break_Dis E_ZDis[5];
extern Break_Dis E_FDis[5];
extern Break_Dis *E_Dis;


typedef struct       //终点会车准备
{
  INT8U element;        //元素类型
  INT8U enable;         //元素使能
  INT8U dir;            //
  INT8U Type;           //
  INT16U length;        //元素多长之后限速
  INT16U LimitV;        //元素限制的速度
  INT16U MaxLength;     //元素最长距离
  INT32S Trace;         //记录这个元素之后走多少米
}LimitElement;
extern  LimitElement E_tra;


extern LimitElement E_Pos[30];     //记录正圈元素
extern LimitElement E_Neg[30];     //记录反圈元素

extern LimitElement E_Pos0[30];     //记录正圈元素
extern LimitElement E_Neg0[30];     //记录反圈元素

extern LimitElement E_Pos1[30];     //记录正圈元素
extern LimitElement E_Neg1[30];     //记录反圈元素

extern LimitElement E_Pos2[30];     //记录正圈元素
extern LimitElement E_Neg2[30];     //记录反圈元素

extern LimitElement E_Pos3[30];     //记录正圈元素
extern LimitElement E_Neg3[30];     //记录反圈元素

extern LimitElement *E_Car;     //记录对方的元素

extern INT8U CircleFlag;


#define FLASH_SECT2_CNT                 1024            //Flash记录正圈的数据
#define FLASH_SECT3_CNT                 1024            //Flash记录反圈的数据
//typedef struct 
//{
//  INT8U  RingNum;        //环的数目  1   bytes
//  INT8U  BreakNum;       //断路数目  1   bytes
//  INT8U  RingType[8];    //环的类型  8   bytes
//  INT8U  FakeCross;     //会车假十字 1   bytes 
//  LimitElement E_tra;
//}Flash_CirCle;
//extern Flash_CirCle sFlash_Pos,sFlash_Neg;      //正反圈
//
//


/******************************************************************************/
// 宏定义
#define KSHOW_COL_ST		1//
#define	KSHOW_ROW_LINE		16//64 =16*4行
#define	KSHOW_LINE_INFO		3
#define	KSHOW_LINE_INPUT	4

#define	KShowLineClear(line)	{OLED_RAM_Fill_add_ClearLine(line);\
									OLED_RAM_Refresh_add_Line(line);}

#define	KShowLineStr(line,str)	{OLED_RAM_Fill_add_ClearLine(line);\
								OLED_RAM_disp_string_add(KSHOW_COL_ST,(line-1)*KSHOW_ROW_LINE,str,FONT_SIZE_L,1);\
								OLED_RAM_Refresh_add_Line(line);}

#define	KShowLineSmallStr(line,str)	{OLED_RAM_Fill_add_ClearLine(line);\
								OLED_RAM_disp_string_add(KSHOW_COL_ST,(line-1)*KSHOW_ROW_LINE,str,FONT_SIZE_S,1);\
								OLED_RAM_Refresh_add_Line(line);}

#define KShowLineNum(line,num)	{OLED_RAM_Fill_add_ClearLine(line);\
								OLED_RAM_disp_num_add(KSHOW_COL_ST,(line-1)*KSHOW_ROW_LINE,num,FONT_SIZE_L,1);\
								OLED_RAM_Refresh_add_Line(line);}

#define KShowLineSmallNum(line,num)	{OLED_RAM_Fill_add_ClearLine(line);\
								OLED_RAM_disp_num_add(KSHOW_COL_ST,(line-1)*KSHOW_ROW_LINE,num,FONT_SIZE_S,1);\
								OLED_RAM_RefresZh_add_Line(line);}

#define	KShowLineStrInput(str)	KShowLineStr(KSHOW_LINE_INPUT,str)
#define KShowLineSmallStrInput(str) KShowLineSmallStr(KSHOW_LINE_INPUT,str)

#define	KShowLineNumInput(num)	KShowLineNum(KSHOW_LINE_INPUT,num)
#define	KShowLineSmallNumInput(num)	KShowLineSmallNum(KSHOW_LINE_INPUT,num)

#define	KShowLineInputClr()		KShowLineClear(KSHOW_LINE_INPUT)

#define	KShowLineStrInfo(str)	KShowLineStr(KSHOW_LINE_INFO,str)
#define	KShowLineSmallStrInfo(str)	KShowLineSmallStr(KSHOW_LINE_INFO,str)

#define	KShowLineNumInfo(num)	KShowLineNum(KSHOW_LINE_INFO,num)
#define	KShowLineSmallNumInfo(num)	KShowLineSmallNum(KSHOW_LINE_INFO,num)


#define KShowLineInfoClr()		KShowLineClear(KSHOW_LINE_INFO)


//Flash
typedef struct
{
	uint8 flashflag;
	uint16 flashdata;
}FLASHPARA;						//占3个byte

#define	   FLASH_SECT1_CNT	128
//#define	FLASH_SECT2_CNT			 4
extern struct Flash_Data
{
  	//Sector 1
	uint16 LightShieldTime;
    uint16 MotorStopTime;
    uint16 MotorStopSpeed;
    uint16 BridgePwm;
	uint16 OnTheBridgeTime;
	uint16 DownBridgeTime;
	uint16 BridgeShieldingTime;
}sFlash_Data;
extern struct Flash_Data sFlash_Data;

extern INT8U CircleNum;
// 外部变量


//12th jys
extern INT16S loca_D_Y;
extern INT16S loca_C_Y;

uint16 ZLG7290GetKeyAndDisplay(void);

// 策略
void Choise(void);              // 0 GO

void Set_Binary(void);          //5
void Set_Turn(void);//3

void Set_Path(void);//11
void Look_Inductance(void);
void Look_AngleDis(void);
void Look_Ring_gyo1(void);
void Set_Every(void);
void Set_g_diff_p(void);
void Set_g_diff_d(void);
void Set_g_SendPic(void);
void Set_g_speed_final(void);
void Set_SwTurn_p(void);
void Set_BreakR(void);
void Set_ToLiangBreak(void);
void Set_HengR(void);
void Set_BigBRIAngle2(void);
void ReadPosCircle();
void ReadNegCircle();
void WritePosCircle();
void WriteNegCircle();
void ClearStr();
void Set_Circle();
void Set_StopDistance(void);
void Set_MyStopDistance(void);
void printStr(const char * p,int num,int line) ;
void See_SeeCarPic(void);
void Set_StopMotorPwm(void);
void Set_FuzzyPara(void);
void Set_Break_Dis(void);
void Set_SlowSwitch(void);
void Set_B_FARJUDGEDIS(void);
void Set_HengDis(void);
void Set_ReedSwitch(void);
//void see(void);
//void sudu(void);
//void POWER(void);

//flash

 
//void Flash_read_YH(void);         //60
//void Flash_write_YH(void);       // 66 


void Flash_read_All(void);		// 77
void Flash_write_All(void);		// 70


//void See_SeeCarPic(void);		// 98
void See_SeeEdge(void) ;		// 98




void Set_motor(void);//9
void Set_DoubleCar(void);
void Set_gyro(void);
void ShowVoltage(int line, int v);//显示电量
void Set_pwmtest(void);
void Set_FollowStopDis(void);
void Set_Angle_P(void);
void Set_Angle_D(void);
void Set_SpeedTime(void);
void LookSpeed(void);
void Set_STOPTEMP(void);
void Set_Gyro_Local(void);
/******************************************************************************/

#endif /* __KEYBOARD_H__ */
