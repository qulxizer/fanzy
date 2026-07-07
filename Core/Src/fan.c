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

void fan_set_duty(temperature_t temp) {
  if (temp < FAN_TEMP_ON_C) {
    fan_pwm_set_raw(0);
    return;
  }
  float ratio = (temp - FAN_TEMP_ON_C) / (FAN_TEMP_FULL_C - FAN_TEMP_ON_C);
  if (ratio < 0.0f)
    ratio = 0.0f;
  if (ratio > 1.0f)
    ratio = 1.0f;

  int duty = FAN_MIN_DUTY + (int)(ratio * (FAN_MAX_DUTY - FAN_MIN_DUTY));

  if (ac_signal_read() == GPIO_PIN_RESET) {
    duty = duty * AC_MULTIPLIER;
    if (duty < AC_MIN_SPEED) {
      duty = AC_MIN_SPEED;
    }
    fan_pwm_set_raw(duty);
  } else {
    fan_pwm_set_raw(duty);
  }
}
