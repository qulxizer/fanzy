/*
 * temp.c
 *
 *  Created on: Mar 30, 2023
 */

/*
 * @brief  Read the temperature from the NTC thermistor
 *
 */
#include "temp.h"
#include "adc.h"
#include "config.h"
#include "stm32g0xx_hal_def.h"
#include <math.h>
#include <stdint.h>
#include <sys/cdefs.h>

const ntc_point_t ntc_table[] = {
    {-40.0f, 102122.0f}, {-35.0f, 73340.0f}, {-30.0f, 53249.0f},
    {-25.0f, 39064.0f},  {-20.0f, 28939.0f}, {-15.0f, 21637.0f},
    {-10.0f, 16321.0f},  {-5.0f, 12413.0f},  {0.0f, 9516.0f},
    {5.0f, 7354.0f},     {10.0f, 5728.0f},   {15.0f, 4496.0f},
    {20.0f, 3555.0f},    {25.0f, 2830.0f},   {30.0f, 2268.0f},
    {35.0f, 1828.0f},    {40.0f, 1483.0f},   {45.0f, 1210.0f},
    {50.0f, 992.0f},     {55.0f, 819.0f},    {60.0f, 679.0f},
    {65.0f, 566.0f},     {70.0f, 475.0f},    {75.0f, 400.0f},
    {80.0f, 338.0f},     {85.0f, 287.0f},    {90.0f, 244.8f},
    {95.0f, 209.7f},     {100.0f, 180.3f},   {105.0f, 155.6f},
    {110.0f, 134.7f},    {115.0f, 117.1f},   {120.0f, 102.2f},
    {125.0f, 89.4f},     {130.0f, 78.5f},    {135.0f, 69.1f},
    {140.0f, 61.1f},     {145.0f, 54.1f},    {150.0f, 48.1f},
};
const size_t ntc_table_size = sizeof(ntc_table) / sizeof(ntc_table[0]);

static temp_status_t ntc_temp_from_resistance(const resistance_t *resistance,
                                              temperature_t *temp) {
  if (resistance == NULL || temp == NULL) {
    return TEMP_ERR_NULL;
  }

  resistance_t r = *resistance;

  if (r >= ntc_table[0].resistance) {
    *temp = ntc_table[0].temp_c;
    return TEMP_ERR_OUT_OF_RANGE;
  }

  if (r <= ntc_table[ntc_table_size - 1].resistance) {
    *temp = ntc_table[ntc_table_size - 1].temp_c;
    return TEMP_ERR_OUT_OF_RANGE;
  }

  for (size_t i = 0; i < ntc_table_size - 1; i++) {
    const ntc_point_t *a = &ntc_table[i];
    const ntc_point_t *b = &ntc_table[i + 1];

    if (r <= a->resistance && r >= b->resistance) {
      float ratio = (r - a->resistance) / (b->resistance - a->resistance);

      *temp = a->temp_c + ratio * (b->temp_c - a->temp_c);
      return TEMP_OK;
    }
  }

  return TEMP_ERR_OUT_OF_RANGE;
}

static temp_status_t adc_read_temp_channel(uint16_t *adc) {
  if (adc == NULL) {
    return TEMP_ERR_NULL;
  }

  if (HAL_ADC_Start(&hadc1) != HAL_OK) {
    return TEMP_ERR_ADC;
  }

  if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK) {
    HAL_ADC_Stop(&hadc1);
    return TEMP_ERR_ADC;
  }

  *adc = (uint16_t)HAL_ADC_GetValue(&hadc1);

  HAL_ADC_Stop(&hadc1);

  return TEMP_OK;
}
static float ntc_resistance_from_adc(uint16_t adc) {
  if (adc == 0)
    return INFINITY;
  if (adc >= TEMP_ADC_MAX)
    return 0.0f;

#if TEMP_DIVIDER_PULLUP
  // VCC -- fixed resistor -- ADC -- NTC -- GND
  return TEMP_R_FIXED_OHM * ((float)adc / (TEMP_ADC_MAX - (float)adc));
#else
  // VCC -- NTC -- ADC -- fixed resistor -- GND
  return TEMP_R_FIXED_OHM * ((TEMP_ADC_MAX - (float)adc) / (float)adc);
#endif
}
temp_status_t read_temp(temperature_t *temp) {
  if (temp == NULL) {
    return TEMP_ERR_NULL;
  }

  uint16_t adc = 0;
  temp_status_t status = adc_read_temp_channel(&adc);
  if (status != TEMP_OK) {
    return status;
  }

  if (adc <= TEMP_ADC_SHORT_THRESHOLD) {
    return TEMP_ERR_SHORT_CIRCUIT;
  }

  if (adc >= TEMP_ADC_OPEN_THRESHOLD) {
    return TEMP_ERR_OPEN_CIRCUIT;
  }

  const resistance_t r = ntc_resistance_from_adc(adc);
  temperature_t t = {0};
  status = ntc_temp_from_resistance(&r, &t);
  if (status != TEMP_OK) {
    return status;
  }

  if (t < -40.0f || t > 150.0f) {
    return TEMP_ERR_OUT_OF_RANGE;
  }

  *temp = t;
  return TEMP_OK;
}
