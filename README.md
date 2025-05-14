# EthernetTcpDemo for NUCLEO-H723ZG

This STM32 project demonstrates basic Ethernet functionality using **ThreadX** and **NetX Duo** with **DHCP**, status LEDs, and auto-generated code from **STM32CubeMX**.  
The project contains a properly configured `.ioc` file for the NUCLEO-H723ZG board, enabling reproducible hardware setup and CubeMX regeneration.

## ✅ Features

- ThreadX RTOS with Azure RTOS integration
- NetX Duo IP stack with DHCP client
- Automatic Ethernet link detection
- Green LED (`LD1`) lights up on IP assignment
- Red LED (`LD3`) lights up when link is down
- CMake-based build for VS Code and STM32CubeCLT
- Generated with STM32CubeMX using X-CUBE-AZRTOS-H7
- Ready for extension with TCP/HTTP(S) server

## 🛠 Requirements

- **Board:** [NUCLEO-H723ZG](https://www.st.com/en/evaluation-tools/nucleo-h723zg.html)
- **Toolchain:** STM32CubeCLT (GCC), CMake ≥ 3.20
- **Programmer:** STM32CubeProgrammer (CLI or GUI)
- **IDE (optional):** Visual Studio Code + Cortex-Debug
- **CubeMX version:** 6.10+ recommended
- **OS:** macOS (Apple Silicon supported)

## 🚀 Build Instructions

```bash
cmake -S . -B build
cmake --build build -j$(sysctl -n hw.ncpu)