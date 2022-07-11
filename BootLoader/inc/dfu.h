///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright (C) 2020 HanvonUgee Technology Corp. All rights reserved.
//FileName: DFU.h
//Brief: DFU functions header file
//Platform:GD32F350xxx
//Ver: V1.0.0
//Date:2020.07.16
//Author:Liming
//Modify: Felix_wang
//	1.ÕûÀí´úÂë
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DFU_H
#define DFU_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ugee_comm.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------User configurable macros begin---------
#define MAJOR_VERSION 1
#define MINOR_VERSION 0


#define FLASH_MEMORY_SIZE_ADDR    0x1FFFF7E0  // first 4 byte 

#define USER_CONFIG_PAGE_ADDR     0x08003000
#define USER_ISPFLAG_PAGE_ADDR    0x08003400  // first 4 byte 

#define APP_LOADED_ADDR           0x08003800
#define APP_END_ADDR              APP_LOADED_ADDR+FLASH_PAGE_SIZE*FLASH_ErasePageVAL

#define FLASH_PAGE_SIZE           0x400U         
#define FLASH_ErasePageVAL    60  

#define FLASH_BOOT_PAGE_SIZE   12 
#define FLASH_DATA_PAGE_SIZE   2 


#define FLASH_ONE_PAGE_WORD      256     

#define MaxUsbPacketSize 64


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	UINT8 sendStatus;
	UINT8 ep1SendStatus;
	UINT8 ep2SendStatus;
	UINT8 ep3SendStatus;
	UINT8 recvStatus;
}UGUSBStatus_Struct;

typedef enum
{
	eStatus_IDLE,
    eStatus_BUSY,

}UGUSBStatus_Enum;


typedef enum
{
	USER_ISP_FLAG_IDLE,
	USER_ISP_FLAG_SW_UPDATE,
	USER_ISP_FLAG_EREASE,
	USER_ISP_FLAG_END,
}USER_ISPStep_Enum;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern UINT8 ispStatusFlag;

extern UINT8 configInfo[];
extern UINT8 flashPageSize;
extern UINT32 flashEndAddr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void JmpAppFunc(void);

void reset_ugUSBStatus(void);
UINT8 getFlashMemorySize(void);
UINT8 checkIfAppInvalid(void);
UINT8 getISPFlag(void);
void setISPFlag(UINT8 flag);
void getUserConfigInfo(UINT8 *buff);
void Deinit_AllFunc(void);

void usb_TaskFunc(void);
void set_ugUSBStatusSend( UINT8 value,UINT8 ep_id);
UINT8 get_ugUSBStatusSend(UINT8 ep_id);
void set_ugUSBStatusRecv( UINT8 value);
UINT8 get_ugUSBStatusRecv(void);

uint32_t Flash_Erase(uint32_t addr, uint32_t pageNum);
uint32_t Flash_Write(uint32_t addr, uint32_t *pData, uint32_t length);
void Flash_Read(uint32_t addr, uint32_t *pData, uint32_t length);



#endif // DFU_H 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////




