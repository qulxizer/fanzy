import serial
import struct

# Configure this to match your PC's port and STM32 baud rate
SERIAL_PORT = "/dev/ttyACM0"  # Use 'COM3', 'COM4', etc. on Windows
BAUD_RATE = 115200

PROTO_SOF = 0xAA


def process_payload(msg_id, payload_bytes):
    """
    This is where you unpack the raw payload bytes back into data types.
    """
    print(
        f"📦 Received Packet - Msg ID: {msg_id}, Payload Size: {len(payload_bytes)} bytes"
    )

    if msg_id == 0x67:  # Assuming 1 is your MSG_ID_INIT
        # 'I' = uint32 (version), 'I' = uint32 (device_uid)
        # matches your proto_msg_init_t exactly
        try:
            version, uid = struct.unpack("<II", payload_bytes)
            print(f"   ↳ [INIT] Protocol Version: {version}, Device UID: 0x{uid:08X}")
        except struct.error:
            print("   ↳ Error: Payload size mismatch for INIT message")

    elif msg_id == 2:  # Example: Telemetry / Status
        # Customize this based on your other message structures
        print(f"   ↳ Raw Payload Hex: {payload_bytes.hex().upper()}")

    print("-" * 50)


def main():
    print(f"Connecting to {SERIAL_PORT} at {BAUD_RATE} baud...")
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    except Exception as e:
        print(f"Error opening port: {e}")
        return

    print("Listening for packets...")

    while True:
        # State: Look for Start of Frame (SOF)
        sof_byte = ser.read(1)
        if not sof_byte or sof_byte[0] != PROTO_SOF:
            continue  # Keep looking

        # Found SOF! Now read the remaining fixed header fields: msg_id (1 byte) and length (1 byte)
        header = ser.read(2)
        if len(header) < 2:
            continue  # Timeout/partial packet

        msg_id = header[0]
        payload_length = header[1]

        # State: Read exactly the amount of payload bytes specified by the header
        payload = ser.read(payload_length)
        if len(payload) < payload_length:
            print("⚠️ Packet dropped: Timeout waiting for full payload data.")
            continue

        # Successfully captured the exact payload packet
        process_payload(msg_id, payload)


if __name__ == "__main__":
    main()
