#include "gd32f3x0.h"
#include "gpio.h"
#include "ugee_i2c.h"



//IQS624
#define GPIO_I2C_SDA_PORT          	GPIOF
#define GPIO_I2C_SDA_PIN           	GPIO_PIN_7
#define GPIO_I2C_SCL_PORT          	GPIOF
#define GPIO_I2C_SCL_PIN           	GPIO_PIN_6

/*!
    \brief      cofigure the I2C0 GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void ugee_i2c_gpio_config(void)
{
    /* connect PF6 to I2C0_SCL */
    gpio_af_set(GPIO_I2C_SCL_PORT, GPIO_AF_0, GPIO_I2C_SCL_PIN);
    /* connect PF7 to I2C0_SDA */
    gpio_af_set(GPIO_I2C_SDA_PORT, GPIO_AF_0, GPIO_I2C_SDA_PIN);
    /* configure GPIO pins of I2C0 */
    gpio_mode_set(GPIO_I2C_SCL_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_I2C_SCL_PIN);
    gpio_output_options_set(GPIO_I2C_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_I2C_SCL_PIN);
    gpio_mode_set(GPIO_I2C_SDA_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_I2C_SDA_PIN);
    gpio_output_options_set(GPIO_I2C_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_I2C_SDA_PIN);

}


/*!
    \brief      cofigure the I2C0interfaces
    \param[in]  none
    \param[out] none
    \retval     none
*/
void ugee_i2c_config(void)
{
    /* enable I2C1 clock */
    rcu_periph_clock_enable(RCU_I2C1);

    i2c_deinit (I2C1);

    ugee_i2c_gpio_config();

    /* configure I2C clock */
    i2c_clock_config(I2C1, 100000, I2C_DTCY_2);
    /* configure I2C address */
    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, IQS624I2C_ADDRESS);
    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, IQS572I2C_ADDRESS);
    i2c_dualaddr_enable(I2C1, IQS572I2C_ADDRESS);
    /* enable I2C */
    i2c_enable(I2C1);
    /* enable acknowledge */
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);

    //i2c_nvic_config();
}
uint16_t repeat_times = 0;
#define  CHECK_TIMEOUT(i2c_repeat_times, sta,flag)  \
{                                                   \
    repeat_times = i2c_repeat_times;                \
    while(1)                                        \
	{										        \
        if(sta == i2c_flag_get(I2C1, flag))break;   \
            if(0 == repeat_times--)goto i2c_err;    \
    }                                               \
                                                    \
}                                                   \

#define I2C_REPEAT_TIMES    20000
//需要写两次设备地址
ErrStatus ugee_i2c_buffer_read(uint8_t* p_buffer,
                               uint8_t read_address,
                               uint8_t device_address,
                               uint16_t number_of_byte)
{
    /* wait until I2C bus is idle */
    //while(i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, RESET, I2C_FLAG_I2CBSY);
    if(2 == number_of_byte)
    {
        i2c_ackpos_config(I2C1, I2C_ACKPOS_NEXT);
    }

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);

    /* wait until SBSEND bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_SBSEND);

    /* send slave address to I2C bus */ // 发送第一次设备地址
    i2c_master_addressing(I2C1, device_address, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_ADDSEND);

    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);

    /* wait until the transmit data buffer is empty */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_TBE);

    /* enable I2C0*/
    i2c_enable(I2C1);

    /* send the EEPROM's internal address to write to */
    i2c_data_transmit(I2C1, read_address);   //发送寄存器地址

    /* wait until BTC bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_BTC);

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);

    /* wait until SBSEND bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_SBSEND);

    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, device_address, I2C_RECEIVER); //再次发送设备地址

    if(number_of_byte < 3)
    {
        /* disable acknowledge */
        i2c_ack_config(I2C1, I2C_ACK_DISABLE);
    }

    /* wait until ADDSEND bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_ADDSEND);

    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);

    if(1 == number_of_byte)
    {
        /* send a stop condition to I2C bus */
        i2c_stop_on_bus(I2C1);
    }

    /* while there is data to be read */
    while(number_of_byte)
    {
        if(3 == number_of_byte)
        {
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));

            /* disable acknowledge */
            i2c_ack_config(I2C1, I2C_ACK_DISABLE);
        }
        if(2 == number_of_byte)
        {
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));

            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2C1);
        }

        /* wait until the RBNE bit is set and clear it */
        if(i2c_flag_get(I2C1, I2C_FLAG_RBNE))
        {
            /* read a byte from the EEPROM */
            *p_buffer = i2c_data_receive(I2C1); //读取等待的数据

            /* point to the next location where the byte read will be saved */
            p_buffer++;

            /* decrement the read bytes counter */
            number_of_byte--;
        }
    }

    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C1) & 0x0200);

    /* enable acknowledge */
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);

    i2c_ackpos_config(I2C1, I2C_ACKPOS_CURRENT);

    return SUCCESS;

i2c_err:
    i2c_stop_on_bus(I2C1);
    return ERROR;
}

//需要写一次设备地址
ErrStatus ugee_i2c_buffer_write(uint8_t* p_buffer,
                                uint8_t write_address,
                                uint8_t device_address,
                                uint16_t number_of_byte)
{
    /* wait until I2C bus is idle */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, RESET, I2C_FLAG_I2CBSY);

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);

    /* wait until SBSEND bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_SBSEND);

    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, device_address, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_ADDSEND);

    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);

    /* wait until the transmit data buffer is empty */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_TBE);

    /* send the EEPROM's internal address to write to : only one byte address */
    i2c_data_transmit(I2C1, write_address);

    /* wait until BTC bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_BTC);

    /* while there is data to be written */
    while(number_of_byte--)
    {
        i2c_data_transmit(I2C1, *p_buffer);

        /* point to the next byte to be written */
        p_buffer++;

        /* wait until BTC bit is set */
        while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));
    }

    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C1);

    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C1) & 0x0200);

    return SUCCESS;

i2c_err:
    i2c_stop_on_bus(I2C1);
    return ERROR;
}


//需要写两次设备地址
ErrStatus ugee_i2c_buffer_read_iqs572(uint8_t* p_buffer,
                                      uint16_t read_address,
                                      uint8_t device_address,
                                      uint16_t number_of_byte)
{
    /* wait until I2C bus is idle */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, RESET, I2C_FLAG_I2CBSY);

    if(2 == number_of_byte)
    {
        i2c_ackpos_config(I2C1, I2C_ACKPOS_NEXT);
    }

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);

    /* wait until SBSEND bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_SBSEND);

    /* send slave address to I2C bus */ // 发送第一次设备地址
    i2c_master_addressing(I2C1, device_address, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_ADDSEND);

    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);

    /* wait until the transmit data buffer is empty */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_TBE);

    /* enable I2C0*/
    i2c_enable(I2C1);

    /* send the EEPROM's internal address to write to */
    i2c_data_transmit(I2C1, read_address >> 8); //发送寄存器地址

    /* wait until BTC bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_BTC);

    /* send the EEPROM's internal address to write to */
    i2c_data_transmit(I2C1, read_address);   //发送寄存器地址
 
    /* wait until BTC bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_BTC);

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);

    /* wait until SBSEND bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_SBSEND);

    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, device_address, I2C_RECEIVER); //再次发送设备地址

    if(number_of_byte < 3)
    {
        /* disable acknowledge */
        i2c_ack_config(I2C1, I2C_ACK_DISABLE);
    }

    /* wait until ADDSEND bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_ADDSEND);

    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);

    if(1 == number_of_byte)
    {
        /* send a stop condition to I2C bus */
        i2c_stop_on_bus(I2C1);
    }

    /* while there is data to be read */
    while(number_of_byte)
    {
        if(3 == number_of_byte)
        {
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));

            /* disable acknowledge */
            i2c_ack_config(I2C1, I2C_ACK_DISABLE);
        }
        if(2 == number_of_byte)
        {
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));

            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2C1);
        }

        /* wait until the RBNE bit is set and clear it */
        if(i2c_flag_get(I2C1, I2C_FLAG_RBNE))
        {
            /* read a byte from the EEPROM */
            *p_buffer = i2c_data_receive(I2C1); //读取等待的数据

            /* point to the next location where the byte read will be saved */
            p_buffer++;

            /* decrement the read bytes counter */
            number_of_byte--;
        }
    }

    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C1) & 0x0200);

    /* enable acknowledge */
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);

    i2c_ackpos_config(I2C1, I2C_ACKPOS_CURRENT);

    return SUCCESS;

i2c_err:
    i2c_stop_on_bus(I2C1);
    return ERROR;
}

//需要写一次设备地址
ErrStatus ugee_i2c_buffer_write_iqs572(uint8_t* p_buffer,
                                       uint16_t write_address,
                                       uint8_t device_address,
                                       uint16_t number_of_byte)
{
    /* wait until I2C bus is idle */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, RESET, I2C_FLAG_I2CBSY);

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);

    /* wait until SBSEND bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_SBSEND);

    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, device_address, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_ADDSEND);

    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);

    /* wait until the transmit data buffer is empty */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_TBE);

    /* send the EEPROM's internal address to write to */
    i2c_data_transmit(I2C1, write_address >> 8); //发送寄存器地址

    /* wait until BTC bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_BTC);

    /* send the EEPROM's internal address to write to */
    i2c_data_transmit(I2C1, write_address);   //发送寄存器地址

    /* wait until BTC bit is set */
    CHECK_TIMEOUT(I2C_REPEAT_TIMES, SET, I2C_FLAG_BTC);

    /* while there is data to be written */
    while(number_of_byte--)
    {
        i2c_data_transmit(I2C1, *p_buffer);

        /* point to the next byte to be written */
        p_buffer++;

        /* wait until BTC bit is set */
        while(!i2c_flag_get(I2C1, I2C_FLAG_BTC));
    }

    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C1);

    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C1) & 0x0200);

    return SUCCESS;

i2c_err:
    i2c_stop_on_bus(I2C1);
    return ERROR;
}

