# STM32DynamixelController

You can use STM32 to communicate with Dynamixel.

# Prerequisites

This code communicates with Dynamixel using [DYNAMIXEL Communiation Protocol 1.0](https://emanual.robotis.com/docs/en/dxl/protocol1/) and [controll table 2.0](https://emanual.robotis.com/docs/en/dxl/mx/mx-64-2/).

# Operation confirmed
- Chip: STM32F446
- Dynamixel: MX64
- RS-485 transceiver: ST3485

# How to use

## Add cpp and hpp files to your project

Add the following files in `Core/Src` / and `Core/Inc/` of this repository to the same location in your project.

## Include hpp file at your cpp file
```
#include "Dynamixel.hpp"
```

## Create an instance

Set the UART handler and the pin to change the driver mode.

```
Dynamixel mx64(&huart5, GPIOC, GPIO_PIN_13);
```
