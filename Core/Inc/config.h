#ifndef FZ_CONFIG_H
#define FZ_CONFIG_H

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
#define FAN_TEMP_FULL_C 100.0f

#define FAN_MIN_DUTY 20.0f
#define FAN_MAX_DUTY 90.0f

// AC CONFIG
#define AC_PULLUP 1
#define AC_MULTIPLIER 2.0f

#endif // !FZ_CONFIG_H
