/*
 * temp.c
 *
 *  Created on: Jan 16, 2026
 *      Author: Mahdi
 */


#define NTC_R0        10000.0f   // 10k at 25C
#define NTC_T0        298.15f    // 25C in Kelvin
#define NTC_BETA      3950.0f    // common value
#define SERIES_R      1000.0f    // your 1k resistor
#define ADC_MAX       4095.0f
#define VREF          3.3f

#include <math.h>
#include "adc.h"

float read_temp(void)
{
    uint32_t adc;

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    adc = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    if (adc == 0 || adc >= ADC_MAX)
        return -273.15f; // invalid

    // ADC → voltage
    float v = (adc / ADC_MAX) * VREF;

    // voltage → resistance
    float r_ntc = SERIES_R * ((VREF / v) - 1.0f);

    // Beta equation
    float tempK = 1.0f / (
        (1.0f / NTC_T0) +
        (1.0f / NTC_BETA) * logf(r_ntc / NTC_R0)
    );

    return tempK - 273.15f; // Kelvin → Celsius
}
