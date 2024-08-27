# To receive data from the gpiobit driver
#
# Author: Pravin Raghul S

class Protoc:

    def __init__(self, clkp, datap):
        self.clkp = clkp
        self.datap = datap

    def _wait_until_last_transaction(self):
        while self.clkp.value() == 1:
            pass

    def read_byte(self):
        byte = 0
        index = 0
        new_state = 0
        old_state = 0

        self._wait_until_last_transaction()

        while True:
            new_state = self.clkp.value()
            if new_state != old_state:
                if new_state: # only if the clock is high
                    byte |= (self.datap.value() << index)
                    index += 1
                if index == 8:
                    break
                old_state = new_state

        return byte