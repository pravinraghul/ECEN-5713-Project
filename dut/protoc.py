# To receive data from the gpiobit driver
#
# Author: Pravin Raghul S

class Protoc:

    def __init__(self, clkp, datap):
        self.clkp = clkp
        self.datap = datap

    def read_byte(self):
        byte = 0
        index = 0
        new_state = 0
        old_state = 0
        while True:
            new_state = self.clkp.value()
            if new_state != old_state:
                if new_state: # only if the clock is high
                    byte |= (self.datap.value() << index)
                    index += 1
            if index == 8:
                return byte
            old_state = new_state