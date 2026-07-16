# LED Control System (AUTOSAR-inspired)

A mini Automotive Embedded project demonstrating UART communication between a Qt HMI application and an NXP S32K144 ECU using an AUTOSAR-inspired layered architecture.

---

## Project Overview

This project consists of two independent applications:

```
LED_Control
│
├── ecu/
│   └── LED_Control_ECU
│
└── qt_hmi/
    └── LED_Control_HMI
```

The Qt application acts as a Human Machine Interface (HMI), while the S32K144 firmware acts as the Electronic Control Unit (ECU).

Communication is performed through UART over the J-Link Virtual COM Port.

```
+---------------------+
|      Qt HMI         |
|---------------------|
|  Button ON / OFF    |
|  Brightness Slider  |
|  Communication Log  |
+----------+----------+
           |
           | UART (9600 baud)
           |
+----------v----------+
|     S32K144 ECU     |
|---------------------|
| Communication       |
| RTE                 |
| Application         |
| IoHwAb              |
| MCAL                |
+----------+----------+
           |
           |
      PWM Output
           |
         LED
```

---

# Features

## Qt HMI

- Scan available COM ports
- Connect / Disconnect serial port
- ON / OFF LED control
- Brightness control (0~100%)
- UART communication log
- Automatic synchronization between slider and LED state
- Modern responsive UI

---

## ECU Firmware

Implemented using an AUTOSAR-inspired software architecture.

```
Application
    │
    ▼
RTE
    │
    ▼
Services
    │
    ▼
ECU Abstraction
    │
    ▼
MCAL
    │
    ▼
S32K144 Hardware
```

### MCAL

- Clock Driver
- UART Driver
- PWM Driver
- DIO Driver

### ECU Abstraction

- LED Driver

### Services

- UART Communication Service
- Command Parser

### Application

- LED Control Software Component

---

# Communication Protocol

Qt sends ASCII commands.

Turn LED ON

```
$LED,ON,100
```

Turn LED OFF

```
$LED,OFF,0
```

Set brightness

```
$LED,ON,35
```

ECU responses

```
$READY

$ACK,ON,35

$ACK,OFF,0

$NACK
```

UART configuration

| Parameter | Value |
|------------|-------|
| Interface | LPUART1 |
| TX | PTC7 |
| RX | PTC6 |
| Baud Rate | 9600 |
| Data Bits | 8 |
| Stop Bits | 1 |
| Parity | None |

---

# Hardware

MCU

- NXP S32K144

Debugger

- SEGGER J-Link

Communication

- J-Link Virtual COM Port

LED

- PTD15 (FTM0_CH0 PWM)

---

# Software

Firmware

- S32 Design Studio 3.5
- GCC ARM Embedded

Desktop

- Qt Creator
- Qt 6
- Qt Widgets
- Qt SerialPort

---

# Project Structure

```
LED_Control
│
├── ecu
│   └── LED_Control_ECU
│       │
│       ├── src
│       │   ├── app
│       │   ├── rte
│       │   ├── services
│       │   ├── ecu_abstraction
│       │   └── mcal
│       │
│       └── Project_Settings
│
└── qt_hmi
    └── LED_Control_HMI
        │
        ├── services
        │   └── SerialManager
        │
        ├── mainwindow.ui
        ├── mainwindow.cpp
        └── mainwindow.h
```

---

# Demo Workflow

1. Flash firmware into the S32K144.
2. Open the Qt application.
3. Refresh available COM ports.
4. Connect to the J-Link Virtual COM Port.
5. Receive:

```
$READY
```

6. Press **ON**.

```
Qt
↓

$LED,ON,100

↓

ECU

↓

LED ON
```

7. Adjust brightness using the slider.

```
Qt

↓

$LED,ON,45

↓

ECU

↓

PWM = 45%
```

8. Press **OFF**.

```
Qt

↓

$LED,OFF,0

↓

ECU

↓

LED OFF
```

---

# Future Improvements

- Automatic J-Link COM port detection
- Multiple LED channels
- RGB LED support
- CAN communication
- LIN communication
- FreeRTOS integration
- Full AUTOSAR Classic architecture
- Configuration files (ARXML-inspired)
- Diagnostic service
- Bootloader support

---

# Author

QuangTM

Automotive Embedded Systems

NXP S32K144 • Qt • C • C++ • AUTOSAR-inspired Architecture