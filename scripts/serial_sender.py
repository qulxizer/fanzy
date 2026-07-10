import serial
import struct
import time

# Configure to match your PC's port and STM32 baud rate
SERIAL_PORT = "/dev/ttyACM0"  # Change to '/dev/ttyUSB0', 'COM4', etc.
BAUD_RATE = 115200

PROTO_SOF = 0xAA
MSG_ID_INIT = 1


def send_init_packet(ser):
    # 1. Define your internal message variables
    protocol_version = 1
    device_uid = 0xDEADBEEF  # Simulated hardware UID

    # 2. Pack the variables into raw little-endian binary bytes (<II = two uint32_t)
    # This matches your C struct: proto_msg_init_t
    payload = struct.pack("<II", protocol_version, device_uid)
    payload_length = len(payload)

    # 3. Construct the full packet frame: SOF (1B) + MSG_ID (1B) + LENGTH (1B) + PAYLOAD
    # 'BBB' means 3 unsigned bytes sequentially
    header = struct.pack("BBB", PROTO_SOF, MSG_ID_INIT, payload_length)
    packet = header + payload

    # 4. Fire it over the UART line
    print(f"🚀 Sending Init Packet ({len(packet)} bytes)...")
    print(f"   Raw Bytes (Hex): {packet.hex().upper()}")
    ser.write(packet)


def main():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connected to {SERIAL_PORT}")
        time.sleep(2)  # Small delay to let the connection stabilize

        # Send a test frame
        send_init_packet(ser)

        ser.close()
        print("Done.")
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    main()
