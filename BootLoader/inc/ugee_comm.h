/*!
    \file  usb_delay.h
    \brief usb delay driver header file

    \version 2017-06-06, V1.0.0, firmware for GD32F3x0
    \version 2019-06-01, V2.0.0, firmware for GD32F3x0
*/

/*
    Copyright (c) 2019, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef UGEE_COMM_H
#define UGEE_COMM_H

#include "gd32f3x0.h"
#include "GenericTypeDefs.h"
#include "dfu.h"
#include "main.h"

#define BPG1010L_A_UG902    0
#define BPG1101L_A_UG902    1
#define BOOT_TYPE           BPG1010L_A_UG902

#if ( BOOT_TYPE == BPG1010L_A_UG902 )
#define GPIO_LED_PORT                     GPIOB
#define LED_PIN                           GPIO_PIN_11
#define LED_R_PIN                           GPIO_PIN_10
#define RCC_AHBPeriph_GPIO_LED            RCU_GPIOB

#define KEYBOOT_PIN                         GPIO_PIN_4
#define KEYBOOT_GPIO_PORT                   GPIOB
#define RCC_AHBPeriph_GPIO_KEYBOOT					RCU_GPIOB

#elif ( BOOT_TYPE == BPG1101L_A_UG902 )
#define GPIO_LED_PORT                     GPIOB
#define LED_R_PIN                           GPIO_PIN_10
#define LED_PIN                           GPIO_PIN_12
#define RCC_AHBPeriph_GPIO_LED            RCU_GPIOB

#define KEYBOOT_PIN                         GPIO_PIN_15
#define KEYBOOT_GPIO_PORT                   GPIOA
#define RCC_AHBPeriph_GPIO_KEYBOOT					RCU_GPIOA
#endif

#define LED2_PIN                         GPIO_PIN_11
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK                    RCU_GPIOB



#define GPIO_BOOT                         GPIOB//GPIOA
#define BOOT_PIN                           GPIO_PIN_0//GPIO_PIN_8
#define RCC_AHBPeriph_GPIO_BOOT            RCU_GPIOB//RCU_GPIOA


void gpio_config(void);


#endif /* UGEE_COMM_H */

