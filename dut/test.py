# Test script to receive a byte
#
# Author: Pravin Raghul S

from machine import Pin
from protoc import Protoc

clkp = Pin(13, Pin.IN)
datap = Pin(12, Pin.IN)
proto = Protoc(clkp, datap)

while True:
    byte = proto.read_byte()
    print("value : ", byte)