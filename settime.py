import time
import serial

ard = serial.Serial(port='/dev/ttyACM0', baudrate=9600, timeout=.1)

ard.write(bytes(str(int(time.time())), 'utf-8'))

time.sleep(5)

ard.write(bytes(str(int(time.time())), 'utf-8'))
