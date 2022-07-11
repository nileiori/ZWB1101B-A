///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright (C) 2020 HanvonUgee Technology Corp. All rights reserved.
//FileName: gd32f3x0_it.h
//Brief: the header file of the interrupt service routines
//Platform:GD32F350xxx
//Ver: V1.0.0
//Date:2020.07.15
//Author: 
//Modify: Felix_wang
//	1.ÕûÀí´úÂë
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GD32F3X0_IT_H
#define GD32F3X0_IT_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "gd32f3x0.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// function declarations 
// this function handles NMI exception 
void NMI_Handler(void);
// this function handles HardFault exception 
void HardFault_Handler(void);
// this function handles MemManage exception 
void MemManage_Handler(void);
// this function handles BusFault exception 
void BusFault_Handler(void);
// this function handles UsageFault exception 
void UsageFault_Handler(void);
// this function handles SVC exception 
void SVC_Handler(void);
// this function handles DebugMon exception 
void DebugMon_Handler(void);
// this function handles PendSV exception 
void PendSV_Handler(void);
// this function handles SysTick exception 
void SysTick_Handler(void);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // GD32F3X0_IT_H
