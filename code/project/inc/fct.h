#ifndef __FCT_H__
#define __FCT_H__


#include "config.h"
#include "..\src\FlexibleButton\flexible_button.h"

#ifdef  configUse_fct

typedef struct fct_stu
{
    FlagStatus  rdy;
    FlagStatus  test_start;
    FlagStatus  destroy;
    FlagStatus  test_freq;
    
    uint8_t     x_ant;
    uint8_t     y_ant;
    uint8_t     period_idx;
    uint8_t     chrg_cnt;
    
    uint8_t     freq_idx;
    uint8_t     ant_index;
    uint8_t     ant_select;
    uint16_t    ant_try_times;
    uint32_t    adc_val;

    void        (*pwm_open)(uint8_t index);
    void        (*charge)(uint8_t x_ant, uint8_t y_ant);
    uint32_t    (*adc_cap)(struct fct_stu* fct_hd);
} fct_stu_handle;
typedef fct_stu_handle* fct_stu_handle_t;


#ifdef __cplusplus
extern "C" {
#endif


fct_stu_handle_t fct_get_handle(void);
void fct_manage_button(flex_button_t *btn);
void fct_pwm_period_set_by_idx(uint8_t idx);
void fct_single_ant_test(uint8_t x_ant, uint8_t y_ant);
uint8_t fct_is_destroy(void);
void fct_exit(void);
void fct_data_analyze(uint8_t cmd);
void fct_exit_test_mode(void);
uint8_t* fct_enter_test_mode(void);
uint8_t* fct_parse(uint8_t desc_index);

#ifdef configUse_flex_button
#define FCT_LONG_PRESS_PRO( )       
#define FCT_SHORT_PRESS_PRO( x )   
#else
void fct_manage_longPress(void);
void fct_manage_shortPress(uint8_t keyIndex);
#define FCT_LONG_PRESS_PRO       fct_manage_longPress
#define FCT_SHORT_PRESS_PRO   fct_manage_shortPress
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif

