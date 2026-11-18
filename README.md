# ESP32-C3 Useless Box

This project is an implementation of the classic "Useless Box" using an ESP32-C3 Supermini microcontroller. The original code has been optimized for this platform.

## Project Overview

A Useless Box is a simple device whose sole function is to turn itself off. When you flip a switch to turn it on, a mechanical arm emerges from the box and flips the switch back to the off position.

This version adds a modern twist with a proximity sensor, allowing for interaction without even touching the box, and features multiple "personalities" or modes for flipping the switch off.

## Functionality

The Useless Box has two main modes of operation:

### 1. Proximity-Based Interaction

- When a hand is detected near the proximity sensor, the lid of the box opens quickly.
- When the hand is moved away, the lid closes.
- This feature can be temporarily disabled during some of the switch-based sequences.

### 2. Switch-Based Interaction

- When the user flips the physical switch to the "ON" position, the box initiates one of ten different sequences to turn the switch back off.
- A counter tracks the number of times the switch has been flipped, and each count triggers a different response, ranging from slow and deliberate to fast and playful. These include various speeds, delays, and even "peeking" or "clapping" the lid.

## Required Components

- **Microcontroller:** ESP32-C3 Supermini (or a similar ESP32-C3 board)
- **Servos:** 2x SG90 or similar micro servos
- **Sensor:** APDS-9960 Proximity/Gesture Sensor
- **Switch:** 1x Toggle Switch
- **LED:** The built-in LED of the ESP32-C3 is used as a status indicator.
- **Power Supply:** A suitable power supply for the ESP32-C3 and servos (e.g., a 5V USB power adapter).
- **Box:** A 3D printed or custom-made box to house the components.

## Wiring

| Component          | ESP32-C3 Pin |
| ------------------ | ------------ |
| Lid Servo          | GPIO 4       |
| Switch Servo       | GPIO 5       |
| Switch             | GPIO 6       |
| APDS-9960 SDA      | GPIO 8       |
| APDS-9960 SCL      | GPIO 9       |
| Built-in LED       | GPIO 8       |

**Note on Pin Conflicts:** The built-in LED on some ESP32-C3 boards (like the DevKitM-1) is on GPIO 8, which is also used for the I2C SDA line for the proximity sensor. While the LED is only used for status indication and the I2C communication is generally robust, be aware of this potential conflict. If you experience issues with the proximity sensor, you may need to use a different pin for the LED or disable it in the code. On boards like the LOLIN C3 Mini, the built-in LED is on GPIO 7, which would avoid this issue.

**Important:** The APDS-9960 sensor is connected to boot-strapping pins (GPIO 8 and 9). You may need to **unplug the sensor when flashing new firmware** to the ESP32-C3.

## Installation and Setup

This project is configured to be built with [PlatformIO](https://platformio.org/).

1.  **Install PlatformIO:** Follow the instructions on the PlatformIO website to install it for your preferred code editor (e.g., Visual Studio Code).
2.  **Clone the Repository:** Clone this project to your local machine.
3.  **Open in PlatformIO:** Open the `code` directory of this project in Visual Studio Code with the PlatformIO extension installed.
4.  **Build and Upload:**
    -   Connect your ESP32-C3 board to your computer.
    -   PlatformIO should automatically detect the board and the correct COM port.
    -   Click the "Upload" button in the PlatformIO toolbar (usually an arrow icon) to build the project and upload it to your device.

## Code Structure

-   `useless-box.ino`: The main application file containing the `setup()` and `loop()` functions.
-   `config.h`: Contains all the pin definitions and configuration constants.
-   `speed-servo.h` / `.cpp`: A wrapper class for the Servo library to control the speed of the servo motors.
-   `proximity-sensor.h` / `.cpp`: A class to manage the APDS-9960 proximity sensor.
-   `status-led.h` / `.cpp`: A simple class to control the status LED.
-   `SparkFun_APDS9960_ESP8266.h` / `.cpp`: A modified third-party library for the APDS-9960 sensor, adapted for ESP8266 and also compatible with ESP32.
-   `platformio.ini`: The PlatformIO project configuration file, which specifies the board, framework, and library dependencies.

## Credits and Original Project

-   **Original Code:** The code in this project is based on the work of [balassy](https://github.com/balassy/useless-box.git).
-   **3D-Printed Parts:** The 3D model for the box can be found on Thingiverse: [https://www.thingiverse.com/thing:3856965](https://www.thingiverse.com/thing:3856965).
-   **APDS-9960 Library:** `SparkFun_APDS9960_ESP8266.*` is based on the SparkFun library by Shawn Hymel (SparkFun Electronics), released under the Beerware license.

## License

This project is released under the MIT License, see [LICENSE](LICENSE). The code in `code/` additionally carries the original MIT license of György Balássy, see [code/LICENSE](code/LICENSE).
