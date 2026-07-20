#include "config.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_flash.h"
#include "stm32g0xx_hal_flash_ex.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

config_status_t write_cfg(config_t *cfg) {
  FLASH_EraseInitTypeDef erase = {
      .TypeErase = FLASH_TYPEERASE_PAGES,
      .Page = FZ_FLASH_LAST_PAGE_NUM,
      .NbPages = 1,
  };

  uint32_t page_error;

  config_t temp;
  memcpy(&temp, cfg, sizeof(temp));

  HAL_FLASH_Unlock();

  if (HAL_FLASHEx_Erase(&erase, &page_error) != HAL_OK) {
    HAL_FLASH_Lock();
    return HAL_FLASH_GetError();
  }

  uint64_t *data = (uint64_t *)&temp;
  uint32_t addr = FZ_FLASH_LAST_PAGE_ADDR;

  for (uint32_t i = 0; i < sizeof(temp) / 8; i++) {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, data[i]) !=
        HAL_OK) {
      HAL_FLASH_Lock();
      return HAL_FLASH_GetError();
    }
    addr += 8;
  }

  HAL_FLASH_Lock();
  return 0;
}

config_status_t load_config(config_t *cfg) {
  if (cfg == NULL) {
    return CONFIG_ERR_NULL_CONFIG;
  }
  memcpy(cfg, (void *)FZ_FLASH_LAST_PAGE_ADDR, sizeof(config_t));
  return CONFIG_OK;
}

config_status_t init_config(void) {
  config_t fcfg = {};
  load_config(&fcfg);
  if (fcfg.magic == 0x46415A59u) {
    return CONFIG_OK;
  }
  config_t cfg = {.magic = CONFIG_MAGIC,
                  .ac_min_speed = AC_MIN_SPEED,
                  .ac_multiplier = AC_MULTIPLIER,
                  .ac_pullup = AC_PULLUP,
                  .fan_pwm_inverted = FAN_PWM_INVERTED,
                  .fan_max_duty = FAN_MAX_DUTY,
                  .fan_min_duty = FAN_MIN_DUTY,
                  .fan_temp_full_c = FAN_TEMP_FULL_C,
                  .fan_temp_on_c = FAN_TEMP_ON_C,
                  .temp_adc_max = TEMP_ADC_MAX,
                  .temp_adc_min = TEMP_ADC_MIN,
                  .temp_adc_open_threshold = TEMP_ADC_OPEN_THRESHOLD,
                  .temp_adc_short_threshold = TEMP_ADC_SHORT_THRESHOLD,
                  .temp_max_valid_c = TEMP_MAX_VALID_C,
                  .temp_min_valid_c = TEMP_MIN_VALID_C,
                  .temp_divider_pu = TEMP_DIVIDER_PULLUP,
                  .temp_r_fixed_ohm = TEMP_R_FIXED_OHM};
  return write_cfg(&cfg);
  HAL_FLASH_Lock();
}
