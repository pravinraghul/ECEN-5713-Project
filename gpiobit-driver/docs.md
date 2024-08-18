## Overview

The GPIO-bit driver will use Simplex (unidirectional) tranmission mode.

## GPIO Bit Pinouts mappings

| Pins   | Function |
| ------ | -------- |
| GPIO21 | Clock    |
| GPIO26 | Data     |

## Protocol Design

When the clk pin is set high, the data pin status is considered as bit value. for example., 

![protocol-design.png](https://github.com/pravinraghul/ECEN-5713-Project/blob/master/gpiobit-driver/protocol-design.png)