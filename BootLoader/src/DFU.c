///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright (C) 2020 HanvonUgee Technology Corp. All rights reserved.
//FileName: DFU.c
//Brief: DFU functions
//Platform:GD32F350xxx
//Ver: V1.0.0
//Date:2020.07.16
//Author:Liming
//Modify: Felix_wang
//	1.整理代码
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "main.h"
#include "usbd_int.h"
#include "hid_core.h"
#include "gd32f3x0_fmc.h"
#include "systick.h"
#include "ugee_comm.h"
#include "string.h"
#include "main.h"
#include "Framework.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define    HAL_FLASH_Unlock             fmc_unlock
#define    HAL_FLASH_Lock               fmc_lock
#define    FLASH_PageErase              fmc_page_erase
#define    HAL_Delay                    delay_1ms
#define    HAL_NVIC_SystemReset         NVIC_SystemReset
#define    USB_PULLUP_L()               gpio_bit_reset(USB_GPIO_PULLUP, USB_PULLUP_PIN)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void USBD_SendReport(uint8_t *pUsbData, uint16_t length);
int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len);
void USBD_ReceiveFunc(uint8_t *pUSER, uint16_t length);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern usb_core_handle_struct usbhs_core_dev;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t  ReceiveData[MaxUsbPacketSize];
uint8_t  SendData[MaxUsbPacketSize];
uint16_t ReceiveFlag = 0;
UINT8 ispStatusFlag = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// USER CODE BEGIN Variables
// USER CODE END Variables

// Function prototypes
// USER CODE BEGIN FunctionPrototypes

UGUSBStatus_Struct ug_usbStatus;

static UINT8 UsbTxData[FRAMEWORK_BUFF_SIZE];

UINT8 configInfo[20] = {0};

UINT8 flashPageSize = 0;

UINT32 flashEndAddr = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void JmpAppFunc(void)
{
    typedef  void  (*pAppFunction) (void);
    pAppFunction Application;
    uint32_t AppAddress;

#if 0
    //void (*pUserApp)(void);
    //updata:
    //       __asm("CPSID  I");    //关中断     __asm("CPSIE  I");   //开中断

    AppAddress = *(volatile uint32_t*) (APP_LOADED_ADDR + 4); //app address
    Application = (void (*)(void)) AppAddress;
    /* Initialize user application's Stack Pointer */
    __set_PSP(*(volatile uint32_t*) APP_LOADED_ADDR);    //SET进程堆栈指针PSP
    __set_CONTROL(0);                                    //设置CONTROL寄存器的值
    __set_MSP(*(volatile uint32_t*) APP_LOADED_ADDR);    //set主堆栈指针MSP
    Application();
    ///////////////////////////////////Jump to User Application area end/////////////////////////
#else
    // test if user code is programmed starting from address 0x08004000
    if (0x20000000 == ((*(__IO uint32_t*)APP_LOADED_ADDR) & 0x2FFE0000))    //app 有程序
    {
        __asm("CPSID  I");    //关中断     __asm("CPSIE  I");   //开中断

        AppAddress = *(__IO uint32_t*) (APP_LOADED_ADDR + 4);
        Application = (pAppFunction) AppAddress;

        // initialize user application's stack pointer
        __set_MSP(*(__IO uint32_t*) APP_LOADED_ADDR);

        // jump to user application
        Application();
    }
#endif
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ugee_fmc_program(uint32_t address, uint32_t * buf, uint32_t cnt)
{
    uint32_t i = 0;

    // unlock the flash program/erase controller
    fmc_unlock();

    fmc_flag_clear(0);

    address = address;

    // reprogram the previous program address
    while(i < cnt)
    {

        while(fmc_flag_get(FMC_FLAG_BUSY));

        fmc_word_program(address + 4 * i, *(buf + i));
        i++;
        fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    }

    // lock the main FMC after the reprogram operation
    fmc_lock();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t Flash_Write(uint32_t addr, uint32_t *pData, uint32_t length)
{
    uint32_t address, i = 0; //FlashAddress

    HAL_FLASH_Unlock();

    fmc_flag_clear(0);

    for (address = addr; address < addr + length * 4; address += 4)
    {
        while(fmc_flag_get(FMC_FLAG_BUSY));

        //FLASH->CR |= FLASH_CR_PG;         //激活 Flash 编程

        if (fmc_word_program(address, *(pData + i)) == FMC_READY)
        {
            if (*(uint32_t*)address != *(uint32_t*)(pData + i))
            {
                return (2);
            }
            i++;
        }
        else
        {
            // Error occurred while writing data in Flash memory
            return (1);
        }

        fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    }

    HAL_FLASH_Lock();

    return (0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Flash_Read(uint32_t addr, uint32_t *pData, uint32_t length)
{
    uint32_t i = 0, j = 0;

    for (i = addr; i < (addr + length * 4); i += 4)
    {
        *(pData + j) = *(uint32_t*)(i);
        j++;
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  addr:    开始地址    结束地地址==>  addr+FLASH_PAGE_SIZE*pageNum
//  pageNum: 擦除页数
uint32_t Flash_Erase(uint32_t addr, uint32_t pageNum)  //103c8t6 FLASH_PAGE_SIZE = 1K
{
    uint32_t PageAddress;
    fmc_state_enum status = FMC_PGERR;

    HAL_FLASH_Unlock();

    for (PageAddress = addr; PageAddress < addr + FLASH_PAGE_SIZE * pageNum; PageAddress += FLASH_PAGE_SIZE)
    {

        while(fmc_flag_get(FMC_FLAG_BUSY));  //有操作则一直等

        FLASH_PageErase(PageAddress);

        status = fmc_ready_wait((uint32_t)1000U);  //5S

        if(status != FMC_READY)
        {
            // Error occurred while page erase
            //Debug_printf ("Erase error! =>continue\r\n");
            return (1);
            //continue;
        }
    }

    HAL_FLASH_Lock();

    return (0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    \brief      getUserConfigInfo
//    \param[in]  store buff
//    \param[out] none
//    \retval     none
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void getUserConfigInfo(UINT8 *buff)
{
    uint32_t temBuf[6];

    Flash_Read(USER_CONFIG_PAGE_ADDR, temBuf, 2);

    buff[0] = 10;
    buff[1] = 0x03;

    if(temBuf[0] != 0xffffffff)
    {
        buff[2] = temBuf[0] & 0xff;  //Vid low byte
        buff[3] = (temBuf[0] >> 8) & 0xff; //Vid high byte
    }

    if(temBuf[1] != 0xffffffff)
    {
        buff[4] = temBuf[1] & 0xff;  //PID low byte
        buff[5] = (temBuf[1] >> 8) & 0xff; //PID high byte
    }

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 checkIfAppInvalid(void)
{
    if (0x20000000 == ((*(__IO uint32_t*)APP_LOADED_ADDR) & 0x2FFE0000))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 getFlashMemorySize(void)
{
    //uint32_t temBuf[2]={0};

    //Flash_Read(FLASH_MEMORY_SIZE_ADDR,temBuf,1);

    flashPageSize = (*(uint32_t*)FLASH_MEMORY_SIZE_ADDR) & 0xff;

    //flashPageSize = temBuf[0] & 0xff;

    if(flashPageSize > FLASH_BOOT_PAGE_SIZE + FLASH_DATA_PAGE_SIZE)
    {
        flashPageSize = flashPageSize - FLASH_BOOT_PAGE_SIZE - FLASH_DATA_PAGE_SIZE;
        flashEndAddr = APP_LOADED_ADDR + FLASH_PAGE_SIZE * flashPageSize;
    }
    else
    {
        flashPageSize = 0;
        flashEndAddr = 0;
    }

    return flashPageSize;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 getISPFlag(void)
{
    uint32_t temBuf;
    UINT8 uFlag = 0;

    Flash_Read(USER_ISPFLAG_PAGE_ADDR, &temBuf, 1);

    uFlag = temBuf & 0xff;

    return uFlag;
}

uint32_t tmpBuf[FLASH_ONE_PAGE_WORD] = {0};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  setISPFlag(UINT8 value)
{
//	uint32_t temBuf[FLASH_ONE_PAGE_WORD]={0};

    Flash_Read(USER_ISPFLAG_PAGE_ADDR, tmpBuf, FLASH_ONE_PAGE_WORD); //1 page

    tmpBuf[0] = value;

    Flash_Erase(USER_ISPFLAG_PAGE_ADDR, 1);

    if(USER_ISP_FLAG_EREASE == value)
    {
        ugee_fmc_program(USER_ISPFLAG_PAGE_ADDR, tmpBuf, 1);
    }
    else
    {
        ugee_fmc_program(USER_ISPFLAG_PAGE_ADDR, tmpBuf, FLASH_ONE_PAGE_WORD);
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void set_ugUSBStatusRecv( UINT8 value)
{
    ug_usbStatus.recvStatus = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 get_ugUSBStatusRecv(void)
{
    return ug_usbStatus.recvStatus;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void reset_ugUSBStatus(void)
{
    memset(&ug_usbStatus, 0, sizeof(ug_usbStatus));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void set_ugUSBStatusSend( UINT8 value, UINT8 ep_id)
{
    if(ep_id == CUSTOMHID_IN_EP)
    {
        ug_usbStatus.ep1SendStatus = value;
    }
    else if(ep_id == EP2_IN)
    {
        ug_usbStatus.ep2SendStatus = value;
    }
    else if(ep_id == EP3_IN)
    {
        ug_usbStatus.ep3SendStatus = value;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT8 get_ugUSBStatusSend(UINT8 ep_id)
{
    if(ep_id == CUSTOMHID_IN_EP)
    {
        return ug_usbStatus.ep1SendStatus;
    }
    else if(ep_id == EP2_IN)
    {
        return ug_usbStatus.ep2SendStatus;
    }
    else if(ep_id == EP3_IN)
    {
        return ug_usbStatus.ep3SendStatus;
    }
    else
    {
        return ug_usbStatus.sendStatus;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void USBD_SendReport(UINT8 *pUsbData, uint16_t length)
{
    //USBD_CUSTOM_HID_SendReport_FS(pUsbData, length);
    custom_hid_report_send (&usbhs_core_dev, pUsbData, length);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ugee_usbd_ep_tx(UINT8 ep_addr, UINT8 *pbuf, UINT8 buf_len)
{
    usbd_ep_tx (&usbhs_core_dev, ep_addr, pbuf, buf_len);
    set_ugUSBStatusSend(eStatus_BUSY, ep_addr);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Deinit_AllFunc(void)
{
    nvic_irq_disable(TIMER0_BRK_UP_TRG_COM_IRQn);

    nvic_irq_disable(USBFS_IRQn);

    //timer_deinit(TIMER0);

    rcu_periph_clock_disable(RCU_TIMER0);

    //gpio_deinit(GPIO_LED_PORT);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void usb_TaskFunc(void)        //BOOT
{
    static uint16_t iTIME, i = 0;

    UINT TxLen;
    UINT8 *TxPtr;

    if(get_usbCurStatus() != USB_STATUS_CONFIGURED || ispStatusFlag == 0)
    {
        if (++iTIME > 35000)
        {
#ifdef	configUse_SEGGER_RTT
            SEGGER_RTT_printf(0, "\r\nThe type-C port was pulled out,please insert it again and press 'update' button");
#endif
            if(!checkIfAppInvalid())
            {
                if(getISPFlag() == USER_ISP_FLAG_SW_UPDATE)
                {
                    setISPFlag(USER_ISP_FLAG_IDLE);
                    NVIC_SystemReset();
                }
            }

            iTIME = 0;
        }
        if(get_usbCurStatus() != USB_STATUS_CONFIGURED)
        {
            HAL_Delay(1);
            return;
        }
    }
    // Check if bootloader has something to send out to PC.
    TxLen = FRAMEWORK_GetTransmitFrame(UsbTxData);
    // Initialize the transmit pointer.
    TxPtr = &UsbTxData[0];

    while(TxLen)
    {
        while(get_ugUSBStatusSend(CUSTOMHID_IN_EP))
        {
            HAL_Delay(1);
            if(i++ > 1000)
            {
                i = 0;
                break;
            }
            // Wait for USB transmit completion.
        }

        // Send the packet (USB endpoint size is always 64 bytes)
        ugee_usbd_ep_tx(CUSTOMHID_IN_EP, TxPtr, MaxUsbPacketSize);

        if(TxLen > MaxUsbPacketSize)
        {
            TxLen -= MaxUsbPacketSize;// Send pending bytes in next loop.
            TxPtr += MaxUsbPacketSize;// Point to next 64bytes.
            // Probably a wait is needed here, otherwise PC app may miss frames.
            //Wait();
        }
        else
        {
            // No more bytes.
            TxLen = 0;
        }
    }

    if(!get_ugUSBStatusRecv())
    {
        iTIME = 0;
        FRAMEWORK_BuildRxFrame(report_buf, MaxUsbPacketSize);
        usbd_ep_rx(&usbhs_core_dev, CUSTOMHID_OUT_EP, report_buf, USB_CUSTOMHID_DATA_SIZE);
        set_ugUSBStatusRecv(eStatus_BUSY);
    }
    else
    {
        HAL_Delay(1);
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////


