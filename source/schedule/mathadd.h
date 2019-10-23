/***********************************************************************
**  Copyright (c)  Smartcar Lab of USTB
**----------------------------------------------------------------------
**	Module Name: Schedule.h
**	Module Date: 03/09/2007                         
**	Module Auth: Chenxi
**  Description: schedule of system
**----------------------------------------------------------------------
**  Revision History:
**  Date:		month/day/year    
**  Notes:
***********************************************************************/

#ifndef MATH_H
#define MATH_H
/*--------------- I N C L U D E S ------------------------------------*/  

/*-------------- - M A C R O S ----------------------------------------*/

int int_sqrt(INT32U x2);
int int_degree(int x, int y);
int int_cos(int x);
int int_sin(int x);

//con10th
signed int sin_10000(signed long Angle);
signed int cos_10000(signed long Angle);


signed long arccos_10000(signed int value);
signed long arcsin_10000(signed int value);
//signed int tan_10000(signed long Angle);
//float cos_f(float);
//float sin_f(float);
//float tan(float);

#endif  // MATH_H