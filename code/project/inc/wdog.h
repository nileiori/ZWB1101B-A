#ifndef __WDOG_H__
#define __WDOG_H__


#include "config.h"


#ifdef __cplusplus
extern "C" {
#endif

#if defined ( configUse_wdog )

void wwdog_init( void );
void wwdog_feed( void );
void fwdog_init( void );
void fwdog_feed( void );

#endif

#ifdef __cplusplus
}
#endif

#endif



