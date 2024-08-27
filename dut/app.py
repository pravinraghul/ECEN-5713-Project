# Script to test the Onboard peripheral
#
# Author: Pravin Raghul S

import time
from machine import Pin
from protoc import Protoc

clkp = Pin(13, Pin.IN)
datap = Pin(12, Pin.IN)
proto = Protoc(clkp, datap)
led = Pin(2, Pin.OUT)

def test_led():
    print("Test Starts: LED blinks for second")
    led.on()
    time.sleep(1)
    led.off()
    print("Test Completed")

def main():
    while True:
        byte = proto.read_byte()
        print("Test ID : ", byte)
        if byte == ord('1'):
            test_led()