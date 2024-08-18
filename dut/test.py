# Test script to receive a byte
from machine import Pin

clk = Pin(13, Pin.IN)
data = Pin(12, Pin.IN)

new_state = 0
old_state = 0
byte = 0
index = 0

while True:
    new_state = clk.value()
    if new_state != old_state:
        if new_state: # only if the clock is high
            byte |= (data.value() << index)
            index += 1
        if index == 8:
            print("value: ", byte)
            byte = 0
            index = 0
        old_state = new_state