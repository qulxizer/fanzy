# fanzy

**fanzy** is an STM32G0-based intelligent PWM fan controller firmware designed for automotive / custom cooling applications. Originally built for **Bosch** fan control modules, it is also verified to work with **Denso** fan controllers.

---

## 🌟 Key Features

* **PWM Fan Control**: Customizable duty cycle range, configurable PWM inversion (inverted logic for Bosch/Denso modules), and temperature-triggered speed scaling.
* **Controller Support**: Works with Bosch PWM fan modules and Denso fan controllers.
* **NTC Thermistor Temperature Sensing**:
  * Integrated lookup table with linear interpolation (-40°C to +150°C).
  * Configurable pull-up / pull-down circuit topologies.
  * Fault detection for sensor short-circuit and open-circuit conditions.
* **AC Signal Integration**: Dedicated input for Air Conditioning signal engagement with minimum AC fan speed override and speed multiplier.
* **Flash Configuration Storage**: Persistent setup parameters stored directly in STM32 flash memory (`0x08007800`).
* **UART Communication & Configuration Protocol**:
  * Half-duplex serial protocol over USART2.
  * Message handlers to initialize, read, and rewrite configuration parameters on-the-fly.
  * Python utilities in `scripts/` for testing and parameter configuration.

---

## 🛠 Hardware & Peripherals

* **MCU**: STM32G031xx
* **Timer (TIM1)**: Channel 1 configured for PWM signal output to fan driver.
* **ADC (ADC1)**: Single channel reading for NTC thermistor circuit.
* **USART (USART2)**: Half-duplex 115200 baud communication.
* **GPIO**: AC signal input with optional pull-up configuration.

---

## ⚙️ Configuration Parameters (`config_t`)

| Parameter | Default | Description |
| :--- | :--- | :--- |
| `fan_temp_on_c` | `60.0` °C | Temperature threshold to start spinning the fan |
| `fan_temp_full_c` | `85.0` °C | Temperature threshold for maximum fan speed |
| `fan_min_duty` | `20%` | Minimum PWM duty cycle when fan is active |
| `fan_max_duty` | `90%` | Maximum PWM duty cycle |
| `fan_pwm_inverted` | `1` (true) | Inverts PWM output polarity for active-low controllers |
| `ac_min_speed` | `50%` | Minimum fan duty cycle when AC input is triggered |
| `ac_multiplier` | `2.0` | Fan speed scaling factor when AC is active |
| `temp_r_fixed_ohm` | `1000` Ω | Fixed resistor value in NTC voltage divider |
| `temp_divider_pu` | `0` (false) | `1` for pull-up, `0` for pull-down circuit configuration |

---

## 📁 Repository Structure

```
.
├── Core/
│   ├── Inc/         # Header files (config, fan control, proto, temp, peripheral init)
│   └── Src/         # Main application logic, temperature reading, protocol handler
├── Drivers/         # STM32G0 HAL & CMSIS driver files
├── scripts/         # Python tools for UART protocol interaction & testing
├── CMakeLists.txt   # CMake build configuration
└── README.md        # Project documentation
```

---

## 🚀 Building & Flashing

### Requirements
* ARM GNU Toolchain (`arm-none-eabi-gcc`)
* CMake & Ninja or Make
* OpenOCD or ST-LINK Utility / PyOCD

### Build Steps
```bash
mkdir build && cd build
cmake ..
make -j4
```

---

## 🐍 Python Scripts

Python scripts are located in `scripts/` for testing UART protocol communication:

* `scripts/serial_sender.py`: Demonstrates sending binary configuration & initialization frames over serial (`/dev/ttyACM0`).
* `scripts/serial_parser.py`: Frame parsing and response inspection tool.
