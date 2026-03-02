# Fanzy – Automotive Fan Controller (STM32F103)
___
Embedded fan controller designed for Bosch radiator fan modules commonly used in German vehicles (e.g., Mercedes platforms).
Built on the STM32F1 series using STM32CubeIDE and HAL.

# Overview
___
Fanzy reads coolant temperature via an NTC sensor and generates a PWM control signal to drive an external Bosch fan module.
The high-current switching is handled by the fan controller module — the MCU only provides the control signal.

Target use cases:
- Custom cooling setups
- Engine swaps
- Aftermarket radiator control
- Standalone fan management systems

# Firmware Architecture
___

| Peripheral | Purpose                             |
| ---------- | ----------------------------------- |
| ADC1       | NTC temperature measurement         |
| TIM2       | PWM generation (fan control output) |
| TIM1       | Timing base                         |
| USART3     | Serial debugging / telemetry        |
| GPIO       | Status LED + control signals        |
| RCC        | External HSE clock configuration    |

# Pin Mapping
| Pin  | Function    | Description              |
| ---- | ----------- | ------------------------ |
| PA0  | TIM2_CH1    | PWM output to fan module |
| PA1  | ADC1_IN1    | NTC temperature input    |
| PB10 | USART3_TX   | Debug transmit           |
| PB11 | USART3_RX   | Debug receive            |
| PC13 | GPIO_Output | Status LED               |


# Build & Flash
1. Open fanzy.ioc in STM32CubeIDE
2. Generate code (if needed)
3. Build project
4. Flash using ST-Link
Toolchain: GCC (CubeIDE managed)

# Safety Notice
This project is intended for automotive experimentation and development.
Proper electrical protection, filtering, and transient suppression must be implemented for vehicle deployment.
