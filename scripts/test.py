import serial
import time

with serial.Serial("/dev/ttyACM0", 115200, timeout=1) as ser:
    time.sleep(1)
    ser.reset_input_buffer()

    while True:
        ser.write(b"C")
        ser.flush()
        print("TX: C")

        data = ser.read(1)
        print("RX:", data, "hex=" + data.hex() if data else "timeout")

        time.sleep(1)
