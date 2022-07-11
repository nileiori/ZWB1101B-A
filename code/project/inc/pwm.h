#ifndef __PWM_H__
#define __PWM_H__

#define PWM_PERIOD_TAB_SIZE         40//53
#define PWM_PRE_PERIOD_TAB_SIZE     6

extern void pwm_period_set(uint32_t period);
extern void pwm_period_set_by_idx(uint8_t idx);
extern void pwm_period_set_by_pre_idx(uint8_t idx);

extern uint8_t pwm_period_idx_get_by_pre_idx(uint8_t idx);

#endif

