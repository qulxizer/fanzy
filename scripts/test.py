import serial
import time

with serial.Serial("/dev/ttyACM0", 115200, timeout=0.1) as ser:
    time.sleep(1)

    while True:
        ser.write(b"A")
        ser.flush()
        print("sent A")
        time.sleep(1)
