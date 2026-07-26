#ifndef FZ_PROTO_H
#define FZ_PROTO_H
#include "config.h"
#include <stdint.h>

#define PROTO_SOF 0xAA
#define PROTO_RESP_SOF 0xBB
#define PROTO_VERSION 1
#define PROTO_PAYLOAD sizeof(config_t)

typedef enum {
  PROTO_OK = 0,
  PROTO_ERR_TRANSMIT,
  PROTO_ERR_NULL_PACKET,
  PROTO_ERR_ENABLE_RECEIVER,
  PROTO_ERR_FAILED_TO_FETCH,
  PROTO_ERR_ENABLE_TRANSMITTER
} proto_status_t;

typedef enum : uint8_t {
  PROTO_MSG_INIT = 0x67,
  PROTO_MSG_READ_CONFIG,
  PROTO_MSG_WRITE_CONFIG,
  PROTO_MSG_READ_SENSORS
} proto_msg_t;

typedef struct __attribute__((packed)) {
  uint8_t sof;
  proto_msg_t msg_id;
  uint8_t length;
  uint8_t payload[PROTO_PAYLOAD];
  // uint16_t crc; // :TODO
} proto_packet_t;

typedef struct __attribute__((packed)) {
  uint32_t protocol_version;
  uint32_t device_uid;
} proto_msg_init_t;

typedef struct __attribute__((packed)) {
  bool AC;
  float temperature;
} proto_msg_sensors_t;

proto_status_t handle_msg_init(void);
proto_status_t handle_msg_write_config(proto_packet_t *pkt);
proto_status_t proto_handle_pkt(proto_packet_t *pkt);
#endif // !FZ_PROTO_H
