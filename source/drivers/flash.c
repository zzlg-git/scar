/*
 * File:	flash.c
 * Purpose:	flash write and read
 *
 * Notes:		
 */
#include "common.h"


/******************************************************************************/
/*
function name:  flash_init
parameters:     none
author:         Xian Lee
date:           2012-8-1
description:    initialize FTFL module
*/
void flash_init(void)
{
  SIM_SCGC6 |= SIM_SCGC6_FTFL_MASK;                   // open the clock gate to the FTFL module
  
 // while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));        //wait for the command completing
  
 // FTFL_FSTAT |= FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK; //clear the ACCERR and FPVIOL bit
}
/*
function name:  flash_erase_sector
parameters:     sectNo: relative sector number
author:         Xian Lee
date:           2012-7-6
description:    erase one flash sector
*/
uint8 flash_erase_sector(uint16 sectNo)
{
  union
  {
    uint32 word;
    uint8 byte[4];
  }dest;
  
  dest.word = (uint32)((SECTOR_BASE+sectNo)*(1<<11));
  
  FTFL_FCCOB0 = ERASE_FLASH_SECTOR;
  
  FTFL_FCCOB1 = dest.byte[2];
  FTFL_FCCOB2 = dest.byte[1];
  FTFL_FCCOB3 = dest.byte[0];
  
  if(1 == flash_cmd_launch())
    return 2;
  
  return 0;
}
/*
function name:  flash_write_bytes
parameters:     sectNo: relative sector number
                offset: address offset
                cnt: data length, must be times of 4
                buf[]: source data buffer
author:         Xian Lee
date:           2012-8-1
description:    program bytes to flash
*/
uint8 flash_write_bytes(uint16 sectNo,uint16 offset,uint16 cnt,uint8 buf[])
{
  uint32 size;
  uint32 destaddr;
  
  union
  {
    uint32 word;
    uint8 byte[4];
  }dest;
  
  if(offset%4 != 0)
    return 1;
  
  FTFL_FCCOB0 = PROGRAM_LONGWORD;
  destaddr = (uint32)((SECTOR_BASE+sectNo)*(1<<11)+offset);
  dest.word = destaddr;
  for(size=0;size<cnt;size+=4,dest.word+=4)
  {
    FTFL_FCCOB1 = dest.byte[2];
    FTFL_FCCOB2 = dest.byte[1];
    FTFL_FCCOB3 = dest.byte[0];    
    
    FTFL_FCCOB4 = buf[size+3];
    FTFL_FCCOB5 = buf[size+2];
    FTFL_FCCOB6 = buf[size+1];
    FTFL_FCCOB7 = buf[size+0];
    
    if(1 == flash_cmd_launch())
      return 2;
  }
  
  return 0;
}

/**********************************************************
Function Name:flash_write_int32
Description:flash写32位数据    
Inputs:  None
Outputs: None
Notes:   
***********************************************************/
uint8 flash_write_int32(uint16 sectNo,uint16 offset,int32 buf)
{
  uint32 destaddr;
  uint8 size[4];
  union
  {
    uint32 word;
    uint8 byte[4];
  }dest;
  
  
  size[0] = buf >> 24;
  size[1] = buf >> 16;  
  size[2] = buf >> 8;  
  size[3] = buf;  
  
  FTFL_FCCOB0 = PROGRAM_LONGWORD;
  destaddr = (uint32)((SECTOR_BASE+sectNo)*(1<<11)+offset*4);
  dest.word = destaddr;

  FTFL_FCCOB1 = dest.byte[2];
  FTFL_FCCOB2 = dest.byte[1];
  FTFL_FCCOB3 = dest.byte[0];    
  
  FTFL_FCCOB4 = size[3];
  FTFL_FCCOB5 = size[2];
  FTFL_FCCOB6 = size[1];
  FTFL_FCCOB7 = size[0];
  
  if(1 == flash_cmd_launch())
    return 2;
  
  return 0;
}

/*
function name:  flash_read_bytes
parameters:     sectNo: relative sector number
                offset: address offset
                cnt: data length, must be times of 4
                buf[]: result data buffer
author:         Xian Lee
date:           2012-8-1
description:    read bytes from flash
*/
void flash_read_bytes(uint16 sectNo,uint16 offset,uint16 cnt,uint8 buf[])
{
  uint32 destaddr;
  uint32 size;
  
  destaddr = (uint32)((SECTOR_BASE+sectNo)*(1<<11)+offset);
  
  for(size=0;size<cnt;size+=4,destaddr+=4)
  {
    buf[size+0] = *((uint8*)(destaddr+0));
    buf[size+1] = *((uint8*)(destaddr+1));
    buf[size+2] = *((uint8*)(destaddr+2));
    buf[size+3] = *((uint8*)(destaddr+3));
  }  
}

/**********************************************************
Function Name:flash_write_int32
Description:flash读32位数据    
Inputs:  None
Outputs: None
Notes:   
***********************************************************/
void flash_read_int32(uint16 sectNo,uint16 offset,uint16 cnt,int32 buf[])
{
  uint32 destaddr;
  uint32 size;
  uint8 bytes[4];
  
  destaddr = (uint32)((SECTOR_BASE+sectNo)*(1<<11)+offset);
  
  for(size=0;size<cnt;size++,destaddr+=4)
  {
    bytes[0] = *((uint8*)(destaddr+0));
    bytes[1] = *((uint8*)(destaddr+1));
    bytes[2] = *((uint8*)(destaddr+2));
    bytes[3] = *((uint8*)(destaddr+3));
    buf[size] = (int32)bytes[0]<<24 | (int32)bytes[1]<<16 | (int32)bytes[2]<<8 | (int32)bytes[3];
  }  
}
/*
function name:  flash_cmd_launch
parameters:     none
author:         Xian Lee
date:           2012-8-1
description:    excute FTFL command
*/
uint8 flash_cmd_launch(void)
{
  while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));  //wait for the command completing
  
  if(FTFL_FSTAT & (FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK))
    FTFL_FSTAT |= ERASE_ACCERR_FPVIOL;           //erase the error bits 
  
  FTFL_FSTAT |= FTFL_FSTAT_CCIF_MASK;            //start command
  
  while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK))   
  {
    if(FTFL_FSTAT & (FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK))  //an error detected,return 1
      return 1;
  }
  
  return 0;
}