# Air Quality Monitoring System (Arduino Uno Project)

This project is a complete air-quality monitoring system built on the ATmega328P (Arduino Uno).  
It measures temperature, humidity, CO₂/air quality, PM2.5, and PM10 levels and displays the results on an OLED I2C display, including a simple animated UI.

---

## Main Project Photo
(mainPic.jpg — will be added later)

---

## Team Members

**Nikita Krasilnikov**  
- Created the GitHub repository  
- Wrote the project documentation  
- Implemented the OLED I2C display interface and UI  

**Danat Pustynnikov**  
- Implemented the MQ135 gas sensor module  
- Implemented communication with the SDS018 laser dust sensor  

**Rauf Iusupov**  
- Created the project presentation  
- Edited the demonstration video  
- Implemented the DHT11 temperature & humidity sensor  

---

## Problem Statement and Solution Overview

### Problem  
Indoor air quality directly affects comfort, productivity, and health. Poor ventilation, dust particles, humidity imbalance, and increased CO₂ levels lead to fatigue, headaches, and long-term health issues.  
Many environments lack a simple, low-cost system that shows real-time air quality.

### Solution  
The system uses an embedded microcontroller (ATmega328P) to:

- Read data from multiple air-quality sensors  
- Process and classify environmental parameters (GOOD / NORMAL / BAD)  
- Display real-time values and status on an OLED screen  
- Provide an animated visual indicator for overall air quality  

The MCU allows continuous monitoring without a computer and presents the data in a clear, user-friendly format.

---

## List of Hardware Components

### Sensors
- **DHT11 – Temperature & Humidity Sensor**  
  Measures basic environmental conditions.

- **MQ135 – Air Quality / CO₂ Approximation Sensor**  
  Estimates general air quality based on gas concentration.

- **SDS018 – Laser Dust Sensor (PM2.5 / PM10)**  
  Measures dust concentration using laser scattering for fine particulate matter.

### Other Components
- **OLED I2C Display (SSD1306, 128×64)** — main user interface  
- **Arduino Uno (ATmega328P)** — system controller  
- **Breadboard, jumper wires, USB cable** — assembly and power  

### Why these components  
Each sensor provides a different environmental parameter:  
DHT11 handles temperature/humidity, MQ135 handles gases/CO₂ approximation, and SDS018 measures PM2.5/PM10 dust.  
Together they form a complete, affordable, and reliable indoor air monitoring system.

---

## Wiring Diagram
(will be added later)

---

## Technical Documentation
A complete technical report will be uploaded here:  
**Technical documentation – coming soon**

---

## Project Demonstration Video
(will be added later)

---

## Project Poster
(will be added later)

---

## References
- DHT11 Sensor Datasheet  
- MQ135 Gas Sensor Datasheet  
- SDS018 Laser Dust Sensor Product Manual  
- SSD1306 OLED Controller Datasheet  
- ATMega328P Microcontroller Datasheet  

