#ifndef __DEVICE_H__
#define __DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ugdef.h"


ug_err_t ug_device_register(ug_device_t dev,
                            const char *name,
                            ug_uint16_t flags);


#ifdef __cplusplus
}
#endif

#endif

