# Arduino-Based Flora_Series_Dose-A-Matic

This project is a precision nutrient dosing controller designed for hydroponic and mushroom cultivation systems. It uses an Arduino Nano to control three dosing pumps, selectable modes, and output volumes, with a clear OLED display interface and a rotary encoder for navigation. The system is designed for long-term durability and user-friendly calibration.

---

## 🧠 Features

- Selectable dosing modes: AllPurpose, Grow, Bloom
- Gallon selection: 1–100 gallons
- Individual pump calibration routine (OLED-driven)
- Relay-controlled 12V dosing pumps
- SH1106 OLED + EC11 rotary encoder UI
- Waterproof housing ready
- Calibrated per 5ml dosing resolution

---

## 🔌 Hardware Overview

### Microcontroller

- **Arduino Nano** (powered via 5V buck converter)

### Pumps

- 3x 12V DC Dosing Pumps
- Controlled via 3-channel relay module

### Relay Module

- 3-Channel mechanical relay
- IN1 = Pink
- IN2 = Purple
- IN3 = Green

### Display & Input

- SH1106 OLED (I2C)
- EC11 Rotary Encoder w/ Push Button
- Additional back/confirm buttons on module

### Power

- 12V 5A DC barrel input
- Mini360 buck converter (12V ➜ 5V)
- 5V powers Nano and relay module
- Nano’s 3.3V pin powers the OLED/Encoder module

### Optional Filtering

- 100µF Electrolytic capacitor
- 0.1µF Ceramic capacitor

---

## ⚙️ Wiring Summary

### Arduino Nano Pin Map

| Nano Pin | Function       | Connected To        |
| -------- | -------------- | ------------------- |
| D2       | Encoder A      | EC11                |
| D3       | Encoder B      | EC11                |
| D4       | Confirm Button | Encoder Module      |
| D6       | Back Button    | Encoder Module      |
| D7       | Relay Green    | Relay IN3           |
| D8       | Relay Purple   | Relay IN2           |
| D9       | Relay Pink     | Relay IN1           |
| A4       | I2C SDA        | OLED Module         |
| A5       | I2C SCL        | OLED Module         |
| 5V       | Power Source   | Buck Converter OUT+ |
| GND      | Shared Ground  | All modules         |

### Power Connections

| Source              | Destination         |
| ------------------- | ------------------- |
| 12V Barrel Jack     | Buck VIN, Relay COM |
| Buck Converter OUT+ | Nano 5V, Relay VCC  |
| Buck GND            | Nano GND, Relay GND |
| Nano 3.3V           | OLED/Encoder VCC    |

### Pump Wiring

- Relay COM terminals → 12V+
- Relay NO terminals → Pump +
- Pump – wires → PSU GND
- Flyback diodes (1N4007) across each motor (stripe to +)

---

## 🧪 Calibration Mode

- Select pump using encoder
- Adjust run-time with back button (+50ms steps)
- Press confirm to run pump
- Observe and measure volume dispensed
- Tune time until \~5ml per run

---

## 📁 GitHub Repo Structure

```
/arduino_code          ← Final sketch files
/pcb_design            ← Gerber, KiCad or Flux files
/breadboard_diagram    ← PNG + .fzz wiring diagrams
/images                ← Build photos
/README.md             ← Project overview (this file)
/BOM.csv               ← Parts list for sourcing
```

---
![photo_2025-04-10_23-09-12](https://github.com/user-attachments/assets/a928ea41-6d17-42d9-ba7e-e5a182485979)
---
![photo_2025-04-10_23-09-15](https://github.com/user-attachments/assets/e288d053-fb7e-4e4f-9307-25b7e4c7147b)



---
## 🛠 Future Ideas

- Save calibration values in EEPROM
- Add real-time clock (RTC) for scheduling
- Log dosing events to SD card
- Replace relays with MOSFETs for faster switching
- Fully custom PCB (in progress!)

---

## 📸 License

This project is open source under the MIT License. Attribution appreciated, but not required.

---

Built with 💧 by an enthusiast of automation, control, and DIY excellence.


