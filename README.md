# rcrover 
A DIY remote-controlled rover for Learning CAD, 3D Printing, and Embedded Systems

## Table of Contents

- [Features](#features)
- [Hardware Components](#hardware-components)
- [Software](#software)
- [Getting Started](#getting-started)
- [CAD Files](#cad-files)
- [Future Plans](#future-plans)
- [License](#license)

## Features

- Custom 3D-printed chassis (designed in OnShape)
- Wireless control using NRF24L01 transceivers
- Dual DC motors with L293D motor driver
- Joystick-based manual control
- Arduino development using PlatformIO

## Hardware Components

| Component          | Quantity |
|--------------------|----------|
| Arduino Nano       | 1        |
| Arduino Mega 2560  | 1        |
| L293D Motor Driver | 1        |
| Micro DC Motors    | 2        |
| NRF24L01 Modules   | 2        |
| 2-Axis Joystick    | 1        |
| Battery Pack       | 1        |
| Chassis (3D print) | 1        |

## Software

- **Platform**: [PlatformIO IDE](https://platformio.org/)
- **Firmware**: Written in C++ for Arduino Nano
- **Wireless Communication**: NRF24L01 library

## Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/scottchen10/rcrover.git
cd rcrover
```

### 2. Assemble the Hardware
    Wire the motor driver, and the NRF24L01 module to any appropriate digital pins on the arduino nano and adjust the pin mapping in code as necessary. 

    Ensure a 5V power supply is powering the motor and arduino.

    Assuming the 3D printed chassis is prepared, place the motors in the appropriate spot attached to the drive gear and secure with zip ties as needed.

    Wire the 2-Axis joystick and another NRF24L01 module to the Arduino Mega 2560 and adjust the pin mapping in the Mega 2560 section of the code.

### 2. Install PlatformIO

Install PlatformIO IDE in your code editor (VSCode recommended).
3. Upload the Firmware

    Connect your Arduino Nano via USB.

    Open the project in PlatformIO.

    Select the nanoatmega328 environment and flash to the microcontroller

    Repeat but with the megaatmega2560 environment and the Arduino Mega 2560

### 5. Test the Rover

Power it up and start driving using the joystick!
## CAD Files

All 3D components were designed in OnShape.

[🔗View OnShape Model](https://cad.onshape.com/documents/12d0c673e413079d624da790/w/821ae4069d81f2e3a4ff1f6f/e/b785c43bee72ff9bdb8584b0)
