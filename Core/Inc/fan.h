#ifndef FZ_FAN_H
#define FZ_FAN_H

#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_gpio.h"
#include "temp.h"
#include <stdint.h>
typedef enum {
  FAN_OK = 0,
  FAN_ERR_AC_SIGNAL_INVALID,
  FAN_ERR_PWM_START,
  FAN_ERR_PWM_DUTY_RANGE,
} fan_status_t;

GPIO_PinState ac_signal_read(void);
void fan_set_duty(config_t *cfg, temperature_t temp);
void fan_init(void);

#endif // !FZ_FAN_H
