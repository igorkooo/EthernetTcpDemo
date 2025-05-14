# Ethernet TCP Project with ThreadX and NetX Duo

This STM32 project demonstrates basic use of Ethernet with ThreadX RTOS and NetX Duo networking stack.  
It is built using STM32CubeMX and CMake, and runs on the STM32H723ZG Nucleo board.

## Features

- Uses ThreadX RTOS and NetX Duo middleware
- Detects physical Ethernet link using NetX status check
- Starts a DHCP client when Ethernet is connected
- Turns on green LED (LD1) when IP address is assigned
- Turns on red LED (LD3) when Ethernet is disconnected
- Ready to be extended with TCP Echo Server or HTTP Server

## Requirements

- STM32H723ZG Nucleo board
- STM32CubeMX and STM32CubeCLT (ARM GCC)
- NetX Duo and ThreadX enabled in CubeMX
- CMake 3.20+ and VS Code (optional)
- STM32CubeProgrammer (for flashing)

## Build and Flash

```bash
cmake -S . -B build
cmake --build build -j$(sysctl -n hw.ncpu)
STM32_Programmer_CLI -c port=SWD -w build/EthernetTcpDemo.elf