# EthernetTcpDemo for NUCLEO-H723ZG

This STM32 project demonstrates basic Ethernet functionality using **ThreadX** and **NetX Duo** with **DHCP**, LED status indicators, and auto-generated code from **STM32CubeMX**. The project is configured for the **STM32H723ZG** microcontroller and uses:

- **STM32CubeMX** for initialization (.ioc included)
- **CMake** build system, compatible with **Visual Studio Code**
- **ST-Link** for flashing and debugging
- **USART2** for serial debug output (retargeted via `retarget.c`)
- **DTCMRAM** memory region for DMA descriptors (optimized for Ethernet)
- **NetXDuo** IP stack with **nx_stm32_eth_driver**
- **LAN8742 PHY** with proper initialization (`nx_eth_phy_init()`)

## ✅ Features

- DHCP client obtains IP address automatically on link up
- Green LED (LD1) turns on after successful DHCP configuration
- Red LED (LD3) indicates no Ethernet link
- Blue LED (LD2) blinks on successful Ethernet packet reception (optional)
- All FreeRTOS/ThreadX configuration is handled via Azure RTOS middleware in CubeMX

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