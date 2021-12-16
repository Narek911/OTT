# Omnic Technical Task on STM32 (STM32 Nucleo-64 development board with STM32L476RG MCU)
## Development environment
  * STM32CubeIDE
  * Library: HAL (hardware abstraction layer driver)
  * Middleware: FreeRTOS

## Application Description
This application implements three tasks and using osThreadNew() API functions.

Two tasks "Task01" and "Task02"  are created using osThreadNew(). 
They intended to  handle the requirements of technical tasks.

The task "SystemMonitorTask" is created using osThreadNew(). 
It is intended to provide extra information during debug. It is periodically calls the vTaskList().

The software timer is registered at a periodic interval of 5000 ms.

A Binary semaphore is used to synchronize "Task 01" and "PeriodicTimerCb01".

A queue is used to demonstrate passing data between  tasks.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".

## Hardware and Software environment
  * This application has been tested with STM32 Nucleo-64 development board and can be
    easily tailored to any other supported device and development board.

## How to use
  * In order to make the application work, follow the steps below:
    1. Install STM32CubeIDE on the host PC (please refer to UM2563 "STM32CubeIDE installation guide".) 
    2. Clone the repo https://github.com/Narek911/OTT.git
    3. Connect the STM32 Nucleo-64 development board to the PC via usb cable (please refer to UM1727 "Getting started with STM32 Nucleo board software  development tools".) 
    4. Open the project 'ott-01' in STM32CubeIDE.
    5. Right Click on the project and choose "Build (OpenOCD)" from the "Build Configurations->Set Active" to compile the project.
    6. Build the project by pressing "Ctrl + b"
    7. Rigth Click on the project and choose "ott-01 Debug (OpenOCD)" from the "Debug as-> Debug Configurations..."
    8. Press "Debug" to run the application.
    9. Open the Console tab in STM32CubeIDE to monitor the application logs



