#ifndef FZ_TEMP_H
#define FZ_TEMP_H

typedef float temperature_t;
typedef float resistance_t;

typedef struct {
  temperature_t temp_c;
  resistance_t resistance;
} ntc_point_t;

typedef enum {
  TEMP_OK = 0,
  TEMP_ERR_NULL,
  TEMP_ERR_ADC,
  TEMP_ERR_OPEN_CIRCUIT,
  TEMP_ERR_SHORT_CIRCUIT,
  TEMP_ERR_OUT_OF_RANGE
} temp_status_t;

temp_status_t read_temp(temperature_t *temp);

#endif // !FZ_TEMP_H
