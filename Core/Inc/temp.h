#ifndef FZ_TEMP_H
#define FZ_TEMP_H

#include "config.h"
#include <stddef.h>
typedef float temperature_t;
typedef float resistance_t;

typedef struct {
  temperature_t temp_c;
  resistance_t resistance;
} ntc_point_t;

extern const ntc_point_t ntc_table[];
extern const size_t ntc_table_size;

typedef enum {
  TEMP_OK = 0,
  TEMP_ERR_NULL,
  TEMP_ERR_ADC,
  TEMP_ERR_OPEN_CIRCUIT,
  TEMP_ERR_SHORT_CIRCUIT,
  TEMP_ERR_OUT_OF_RANGE
} temp_status_t;

temp_status_t read_temp(config_t *cfg, temperature_t *temp);

#endif // !FZ_TEMP_H
