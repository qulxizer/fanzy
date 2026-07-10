#ifndef FZ_PROTO_H
#define FZ_PROTO_H
#include "config.h"
#include <stdint.h>

#define PROTO_SOF 0xAA
#define PROTO_VERSION 1
#define PROTO_PAYLOAD sizeof(config_t)

typedef enum {
  PROTO_OK = 0,
  PROTO_ERR_TRANSMIT,
  PROTO_ERR_ENABLE_RECEIVER,
  PROTO_ERR_ENABLE_TRANSMITTER
} proto_status_t;

typedef enum {
  PROTO_MSG_INIT = 0x67,
} proto_msg_t;

typedef struct __attribute__((packed)) {
  uint8_t sof;
  uint8_t msg_id;
  uint8_t length;
  uint8_t payload[PROTO_PAYLOAD];
  // uint16_t crc; // :TODO
} proto_packet_t;

typedef struct __attribute__((packed)) {
  uint32_t protocol_version;
  uint32_t device_uid;
} proto_msg_init_t;

proto_status_t init_proto(void);
#endif // !FZ_PROTO_H
