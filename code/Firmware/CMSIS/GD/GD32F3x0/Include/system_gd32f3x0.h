///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright (C) 2020 HanvonUgee Technology Corp. All rights reserved.
//FileName: system_gd32f3x0.h
//Brief:CMSIS Cortex-M4 Device Peripheral Access Layer Header File for
//           GD32F3x0 Device Series
//Platform:GD32F350xxx
//Ver: V1.0.0
//Date:2020.07.15
//Author: 
//Modify: Felix_wang
//	1.ÕûÀí´úÂë
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


// This file refers the CMSIS standard, some adjustments are made according to GigaDevice chips 

#ifndef SYSTEM_GD32F3X0_H
#define SYSTEM_GD32F3X0_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// system clock frequency (core clock) 
extern uint32_t SystemCoreClock;

// function declarations 
// initialize the system and update the SystemCoreClock variable 
extern void SystemInit (void);
// update the SystemCoreClock with current core clock retrieved from cpu registers 
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif // SYSTEM_GD32F3X0_H 


