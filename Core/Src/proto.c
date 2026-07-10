#include "proto.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_def.h"
#include "stm32g0xx_hal_uart.h"
#include "usart.h"
#include <stdint.h>
#include <string.h>

proto_status_t init_proto(void) {
  HAL_StatusTypeDef st = HAL_HalfDuplex_EnableTransmitter(&huart2);
  if (st != HAL_OK) {
    return PROTO_ERR_ENABLE_TRANSMITTER;
  }

  proto_msg_init_t msg = {.protocol_version = PROTO_SOF,
                          .device_uid = HAL_GetDEVID()};

  proto_packet_t pkt = {0};
  pkt.sof = PROTO_SOF;
  pkt.msg_id = PROTO_MSG_INIT;
  pkt.length = sizeof(msg);
  memcpy(pkt.payload, &msg, sizeof(msg));
  uint16_t tx_size = (sizeof(proto_packet_t) - PROTO_PAYLOAD) + pkt.length;
  st = HAL_UART_Transmit(&huart2, (uint8_t *)&pkt, tx_size, HAL_MAX_DELAY);
  if (st != HAL_OK) {
    return PROTO_ERR_TRANSMIT;
  }
  st = HAL_HalfDuplex_EnableReceiver(&huart2);
  if (st != HAL_OK) {
    return PROTO_ERR_ENABLE_RECEIVER;
  }
  return PROTO_OK;
}
