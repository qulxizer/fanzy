#ifndef FZ_CONFIG_H
#define FZ_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

typedef struct __attribute__((aligned(8))) {
  bool temp_divider_pu;
  bool fan_pwm_inverted;
  bool ac_pullup;
  float temp_r_fixed_ohm;
  float temp_adc_max;
  float temp_adc_min;
  float temp_adc_short_threshold;
  float temp_adc_open_threshold;
  float temp_min_valid_c;
  float temp_max_valid_c;
  float fan_temp_on_c;
  float fan_temp_full_c;
  float ac_multiplier;
  int fan_min_duty;
  int fan_max_duty;
  int ac_min_speed;
} config_t;

// ERRORS
typedef enum { CONFIG_OK = 0, CONFIG_ERR_NULL_CONFIG } config_status_t;

config_status_t init_config(void);
config_status_t load_config(config_t *cfg);
config_status_t write_cfg(config_t *cfg);

// FLASH CONFIG
#define FZ_FLASH_LAST_PAGE_ADDR 0x08007800
#define FZ_FLASH_LAST_PAGE_NUM 15

// TEMP CONFIG

// 1 = pullup, 0 = pulldown
#define TEMP_DIVIDER_PULLUP 0
#define TEMP_R_FIXED_OHM 1000.0f

#define TEMP_ADC_MAX 4095.0f
#define TEMP_ADC_MIN 0.0f

#define TEMP_ADC_SHORT_THRESHOLD 300
#define TEMP_ADC_OPEN_THRESHOLD 3900

#define TEMP_MIN_VALID_C -40.0f
#define TEMP_MAX_VALID_C 150.0f

// FAN CONFIG
#define FAN_PWM_INVERTED 1

#define FAN_TEMP_ON_C 60.0f
#define FAN_TEMP_FULL_C 85.0f

#define FAN_MIN_DUTY 20.0f
#define FAN_MAX_DUTY 90.0f

// AC CONFIG
#define AC_PULLUP 1
#define AC_MIN_SPEED 50.0f
#define AC_MULTIPLIER 2.0f

#endif // !FZ_CONFIG_H
