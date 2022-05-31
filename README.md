# STM32DynamixelController

You can use STM32 to communicate with Dynamixel.

# Prerequisites

This code communicates with Dynamixel using [DYNAMIXEL Communiation Protocol 1.0](https://emanual.robotis.com/docs/en/dxl/protocol1/) and [Controll Table 2.0](https://emanual.robotis.com/docs/en/dxl/mx/mx-64-2/). If the firmware version of your Dynamixel is old, update it to Controll table 2.0.

# Operation confirmed

- MCU: STM32F446RE
- Dynamixel: MX64
- RS-485 transceiver: ST3485 (Circuit example ↓)
  ![image](https://user-images.githubusercontent.com/39794518/171106358-3a219f7b-bb4a-4dd2-91ba-fe9129ae67ee.png)
  
# How to use

## Add cpp and hpp files to your project

Add the following files in `Core/Src`  and `Core/Inc` of this repository to the same location in your project.

*Core/Inc*
- BufData.hpp
- Dynamixel.hpp
- HAL_RS485.hpp

*Core/Src*
- BufData.cpp
- Dynamixel.cpp
- HAL_RS485.cpp

## Define UART handler

Define UART hander in your cpp file.

```
UART_HandleTypeDef huart5;
```

## Include the hpp file at your cpp file
```
#include "Dynamixel.hpp"
```

## Create an instance

Set the UART handler and the pin to change the driver mode.
(For example, in the case of ST3485, the driver mode change pin is connected to the RE pin and DE pin.)

```
Dynamixel mx64(&huart5, GPIOC, GPIO_PIN_13);
```

## *Additional setting

if you get this error, follow the instructions.

```
The float formatting support is not enabled, check your MCU Settings from "Project Properties > C/C++ Build > Settings > Tool Settings", or add manually "-u _printf_float" in linker flags.
```

# Member Functions

## 

