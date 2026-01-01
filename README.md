# Dual-Axis Solar Panel Tracker

![Arduino](https://img.shields.io/badge/Platform-Arduino-blue)
![Servo](https://img.shields.io/badge/Servo-2-red)
![I2C_LCD](https://img.shields.io/badge/LCD-I2C-green)

---

## Overview

This project implements a **dual-axis solar panel tracker** using an Arduino, two servo motors, photoresistors, a push-button, and an I2C LCD.  

- **XY rotation (D10 servo)** → automatically tracks the brightest light.  
- **ZY tilt (D9 servo)** → manually adjusted with a button.  
- **LCD display** → shows real-time readable light levels (`LOW`, `MED`, `HIGH`).  

Perfect for high school STEM projects or a small DIY solar tracker! 🌞⚡

---

## 🛠 Components

| Component               | Arduino Pin | Notes |
|-------------------------|------------|-------|
| Servo D10 (XY rotation) | D10        | Moves horizontally toward light |
| Servo D9 (ZY tilt)      | D9         | Moves vertically, manual button control |
| Push Button             | D2         | Increments D9 by 10° per press, resets at 150° |
| Photoresistor (Front)   | A2         | XY tracking |
| Photoresistor (Middle)  | A1         | XY tracking |
| Photoresistor (Back)    | A3         | XY tracking |
| I2C LCD SDA             | A4         | 16x2 LCD |
| I2C LCD SCL             | A5         | 16x2 LCD |

> **Tip:** Button uses `INPUT_PULLUP`. Connect one side to GND.  
> Photoresistors should be connected with a voltage divider for proper readings.

---

## Features

### XY Rotation – Servo D10
- Automatically points toward **brightest sensor**.  
- Updates every **30 seconds**.  
- Maps positions:  
  - Front → 0°  
  - Middle → 90°  
  - Back → 180°  
- Moves **smoothly** between positions.

### ZY Tilt – Servo D9
- Controlled manually with a **button**.  
- Moves **10° per press**.  
- **Resets to 0° after 150°**.  
- Starts at **60° tilt** for optimal angle.

### LCD Display
- Shows readable light levels: **Front / Middle / Back**.  
- Updates continuously.  
- Example display:  
