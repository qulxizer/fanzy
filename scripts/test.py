import serial
import time

with serial.Serial("/dev/ttyACM0", 115200, timeout=1) as ser:
    time.sleep(1)
    ser.reset_input_buffer()

    while True:
        ser.write(b"A")
        ser.flush()
        byte = ser.read(1)

        if not byte:
            print("timeout")
            break

        print("received:", byte)

        if byte == b"Z":
            print("STM32 reply received")
            break
        time.sleep(1)
