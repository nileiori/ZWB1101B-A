

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TASKSCHEDULE_H
#define __TASKSCHEDULE_H

/* Includes ------------------------------------------------------------------*/
#include "gd32f3x0.h"
#include "config.h"

#define			BIT_SET(Val,bit)			(Val |= 0x1<<bit)
#define			BIT_CLR(Val,bit)			(Val &= ~(0x1<<bit))
#define			BIT_CHECK(Val,bit)		    (Val & 0x1<<bit)

//*****************�¼�����**************************
typedef enum
{
    EV_NOP = 0,

    MAX_EVENTASK

} EVENTTASK;

//******************ʱ������***********************
typedef enum
{
#if !defined(configUse_usart_dma)
    TIME_USART = 0,
#endif
    TIME_SYS_POWER_DOWN,
    TIME_KEY,
    TIME_LED,
    TIME_JUDGE_WORK_MODE,   
#if defined( configUse_ESDTEST )
    //TIME_IQS_COUNTER,
#endif
    TIME_IQS, 
    TIME_COMM_CLR_TILT,
    TIME_LONGPRESS_RESET,
//    TIME_BLE_CONNECT,
    MAX_TIMETASK
} TIMERTASK;
/*------------------------ Execute task by timer--------------------------------
Max task is 32. when power on dev this task is init.
Include
------------------------------------------------------------------------------*/
typedef struct
{
    EventStatus TaskTimerState;       //task valid,1:valid;0:invalid
    EventStatus TaskOverflow;           //task valid,1:valid;0:invalid
    uint32_t TaskTimerCount;           //variables task time count
    uint32_t TaskExTimer;              //set fixed param by independently funtion module
    EventStatus (* operate)(void);     //handle independtly funtion
} TIME_TASK;


/*******************************************************************************
* Function Name  : TimerTaskScheduler
* Description    : Schedule Timer task .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TimerTaskScheduler(void);

/*******************************************************************************
* Function Name  : SetTimerTask
* Description    : Setup timer task .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetTimerTask(TIMERTASK task, uint32_t time);

/*******************************************************************************
* Function Name  : ClrTimerTask
* Description    : Cancel timer task .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ClrTimerTask(TIMERTASK task);
/*******************************************************************************
* Function Name  : ResumeTimerTask
* Description    : resume timer task .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ResumeTimerTask(TIMERTASK task);
/*******************************************************************************
* Function Name  : SuspendTimerTask
* Description    : suspend timer task .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SuspendTimerTask(TIMERTASK task);


#endif
