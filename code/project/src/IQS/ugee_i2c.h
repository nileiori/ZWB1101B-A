#ifndef __UGEE_I2C_H__
#define __UGEE_I2C_H__

#include "config.h"

#define IQS624I2C_ADDRESS         ( 0x44 << 1 )

#define IQS572I2C_ADDRESS         ( 0x74 << 1 )


#ifdef __cplusplus
extern "C" {
#endif


void ugee_i2c_config(void);
ErrStatus ugee_i2c_buffer_read(uint8_t* p_buffer, 
                        uint8_t read_address, 
                        uint8_t device_address, 
                        uint16_t number_of_byte);
ErrStatus ugee_i2c_buffer_write(uint8_t* p_buffer, 
                        uint8_t write_address, 
                        uint8_t device_address, 
                        uint16_t number_of_byte);

ErrStatus ugee_i2c_buffer_read_iqs572(uint8_t* p_buffer,
                     uint16_t read_address,
                     uint8_t device_address,
                     uint16_t number_of_byte);
ErrStatus ugee_i2c_buffer_write_iqs572(uint8_t* p_buffer,
                     uint16_t write_address,
                     uint8_t device_address,
                     uint16_t number_of_byte);


#ifdef __cplusplus
}
#endif

#endif

