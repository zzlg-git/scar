/*
 * File:	flash.h
 * Purpose:	flash 
 *
 * Notes:		
 */

#ifndef __FLASH_H__
#define __FLASH_H__

#define ERASE_FLASH_SECTOR 0x09
#define PROGRAM_LONGWORD 0x06
#define READ_RESOURCE 0x03

#define ERASE_ACCERR_FPVIOL 0x30

#define SECTOR_BASE 240

void flash_init(void);
uint8 flash_erase_sector(uint16 sectNo);
uint8 flash_write_bytes(uint16 sectNo,uint16 offset,uint16 cnt,uint8 buf[]);
void flash_read_bytes(uint16 sectNo,uint16 offset,uint16 cnt,uint8 buf[]);
void flash_read_int32(uint16 sectNo,uint16 offset,uint16 cnt,int32 buf[]);
uint8 flash_cmd_launch(void);
uint8 flash_write_int32(uint16 sectNo,uint16 offset,int32 buf);
#endif /* __FLASH_H__ */