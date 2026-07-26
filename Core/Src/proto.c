#include "proto.h"
#include "config.h"
#include "fan.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_def.h"
#include "stm32g0xx_hal_gpio.h"
#include "stm32g0xx_hal_uart.h"
#include "temp.h"
#include "usart.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

proto_status_t handle_msg_init(void) {
  HAL_StatusTypeDef st = HAL_HalfDuplex_EnableTransmitter(&huart2);
  if (st != HAL_OK) {
    return PROTO_ERR_ENABLE_TRANSMITTER;
  }
  proto_msg_init_t msg = {.protocol_version = PROTO_SOF,
                          .device_uid = HAL_GetDEVID()};

  proto_packet_t pkt = {0};
  pkt.sof = PROTO_RESP_SOF;
  pkt.msg_id = PROTO_MSG_INIT;
  pkt.length = sizeof(msg);
  memcpy(pkt.payload, &msg, sizeof(msg));
  uint16_t tx_size = (sizeof(proto_packet_t) - PROTO_PAYLOAD) + pkt.length;
  st = HAL_UART_Transmit(&huart2, (uint8_t *)&pkt, tx_size, HAL_MAX_DELAY);
  if (st != HAL_OK) {
    return PROTO_ERR_TRANSMIT;
  }
  return PROTO_OK;
}

proto_status_t handle_msg_read_config(proto_packet_t *pkt);
proto_status_t handle_msg_write_config(proto_packet_t *pkt);
proto_status_t handle_msg_read_sensors(proto_packet_t *pkt);
proto_status_t proto_handle_pkt(proto_packet_t *pkt) {
  if (pkt == NULL) {
    return PROTO_ERR_NULL_PACKET;
  }

  HAL_StatusTypeDef st = HAL_HalfDuplex_EnableTransmitter(&huart2);
  if (st != HAL_OK) {
    return PROTO_ERR_ENABLE_TRANSMITTER;
  }
  switch (pkt->msg_id) {
  case PROTO_MSG_INIT:
    return handle_msg_init();
  case PROTO_MSG_READ_CONFIG:
    return handle_msg_read_config(pkt);
  case PROTO_MSG_WRITE_CONFIG:
    return handle_msg_write_config(pkt);
  case PROTO_MSG_READ_SENSORS:
    return handle_msg_read_sensors(pkt);
  }

  return PROTO_OK;
}

proto_status_t handle_msg_read_sensors(proto_packet_t *pkt) {
  config_t cfg = {0};
  load_config(&cfg);
  temperature_t temp = {0};
  temp_status_t st = read_temp(&cfg, &temp);
  if (st != TEMP_OK) {
    return PROTO_ERR_FAILED_TO_FETCH;
  }

  GPIO_PinState ac_status = ac_signal_read();
  proto_msg_sensors_t sens = {.AC = ac_status, .temperature = temp};

  proto_packet_t rpkt = {0};
  rpkt.sof = PROTO_RESP_SOF;
  rpkt.msg_id = PROTO_MSG_READ_SENSORS;
  rpkt.length = sizeof(sens);
  memcpy(rpkt.payload, &sens, sizeof(sens));

  uint16_t tx_size = (sizeof(proto_msg_sensors_t)) + rpkt.length;
  HAL_StatusTypeDef hst =
      HAL_UART_Transmit(&huart2, (uint8_t *)&rpkt, tx_size, HAL_MAX_DELAY);
  if (hst != HAL_OK) {
    return PROTO_ERR_TRANSMIT;
  }

  return PROTO_OK;
}

proto_status_t handle_msg_read_config(proto_packet_t *pkt) {
  config_t cfg = {0};
  load_config(&cfg);

  proto_packet_t rpkt = {0};
  rpkt.sof = PROTO_RESP_SOF;
  rpkt.msg_id = PROTO_MSG_READ_CONFIG;
  rpkt.length = sizeof(cfg);
  memcpy(rpkt.payload, &cfg, sizeof(cfg));
  uint16_t tx_size = (sizeof(proto_packet_t) - PROTO_PAYLOAD) + rpkt.length;
  HAL_StatusTypeDef st =
      HAL_UART_Transmit(&huart2, (uint8_t *)&rpkt, tx_size, HAL_MAX_DELAY);
  if (st != HAL_OK) {
    return PROTO_ERR_TRANSMIT;
  }

  return PROTO_OK;
}

proto_status_t handle_msg_write_config(proto_packet_t *pkt) {
  if (pkt == NULL) {
    return PROTO_ERR_NULL_PACKET;
  }

  config_t cfg;
  memcpy(&cfg, pkt->payload, sizeof(cfg));

  write_cfg(&cfg);

  return PROTO_OK;
}
