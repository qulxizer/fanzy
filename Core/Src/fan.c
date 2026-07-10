#include "config.h"
#include "main.h"
#include "stm32g0xx_hal_gpio.h"
#include "temp.h"
#include "tim.h"

static void fan_pwm_set_raw(uint8_t duty);

void fan_init(void) {
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  fan_pwm_set_raw(0);
}

static GPIO_PinState ac_signal_read(void) {
  return HAL_GPIO_ReadPin(AC_SIG_GPIO_Port, AC_SIG_Pin);
}

static void fan_pwm_set_raw(uint8_t duty) {
  if (duty > 100)
    duty = 100;

#if FAN_PWM_INVERTED
  duty = 100 - duty;
#endif

  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty);
}

void fan_set_duty(config_t *cfg, temperature_t temp) {
  if (temp < cfg->fan_temp_on_c) {
    fan_pwm_set_raw(0);
    return;
  }
  float ratio =
      (temp - cfg->fan_temp_on_c) / (cfg->fan_temp_full_c - cfg->fan_temp_on_c);
  if (ratio < 0.0f)
    ratio = 0.0f;
  if (ratio > 1.0f)
    ratio = 1.0f;

  int duty = cfg->fan_min_duty +
             (int)(ratio * (cfg->fan_max_duty - cfg->fan_min_duty));

  if (ac_signal_read() == GPIO_PIN_RESET) {
    duty = duty * cfg->ac_multiplier;
    if (duty < cfg->ac_min_speed) {
      duty = cfg->ac_min_speed;
    }
    fan_pwm_set_raw(duty);
  } else {
    fan_pwm_set_raw(duty);
  }
}
