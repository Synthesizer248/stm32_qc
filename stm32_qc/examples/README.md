# STM32 QC Controller Library

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

A compact library for implementing QC2.0-QC4.0 voltage negotiation on STM32 microcontrollers with OLED display and rotary encoder interface.

## Features
- QC2.0-QC4.0 voltage profiles
- Rotary encoder navigation
- SSD1306 OLED display support
- Low-level DAC optimization
- Memory-efficient design

## Installation
1. Arduino IDE:  
   `Sketch > Include Library > Manage Libraries... > Search for "stm32_qc"`
2. Manual:  
   Clone this repository to your Arduino libraries folder

## Hardware Requirements
- STM32 Blue Pill or similar
- SSD1306 I2C OLED (128x64)
- Rotary encoder with push button
- QC-enabled device connector

## Wiring
| STM32 Pin | Component  |
|-----------|------------|
| PA0       | Encoder CLK|
| PA1       | Encoder DT |
| PA2       | Encoder SW |
| PA4       | QC D+      |
| PA5       | QC D-      |
| PB6/PB8   | SCL        |
| PB7/PB9   | SDA        |

## License
MIT License - See [LICENSE](LICENSE) for details