# Automated Water Treatment System

A microcontroller-based filtration prototype designed to automate water purification processes using sensors, pumps, and servo motors. Developed with **Arduino**, the system utilizes conditionals and loops to maintain a continuous filtration cycle. Real-time turbidity monitoring and an emergency stop function enhance reliability, making it a scalable solution for rural communities.

## Showcase

![ClosedView](./WaterFiltrationDemos/ClosedView.jpg)

## Features

- **Automated Water Filtration**: Controls pumps and motors to regulate water flow and filtration.
- **Real-time Monitoring**: Reads turbidity sensor data to assess water quality.
- **Emergency Stop Mechanism**: Interrupt-driven system for immediate shutdown.
- **Servo-Controlled Dispensing**: Mechanism to regulate water output.
- **Scalable Design**: Built to be adaptable for larger applications.

## Hardware Components

- **Arduino (Uno/Mega)**
- **AFMotor Shield**
- **DC Motor**
- **Peristaltic Pump**
- **Chemical Pump**
- **Turbidity Sensor**
- **Servo Motor**
- **Push Buttons**
- **Water Level Sensor**

## Software & Libraries

- **Arduino IDE**
- [`AFMotor`](https://github.com/adafruit/Adafruit-Motor-Shield-library) – Motor control library.
- [`Servo`](https://www.arduino.cc/reference/en/libraries/servo/) – Servo motor library.

## Installation & Usage

1. **Clone the repository:**
   ```sh
   git clone https://github.com/justinlam6/water-treatment-system.git
   cd water-treatment-system
