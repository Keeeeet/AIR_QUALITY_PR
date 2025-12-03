# Air Quality Monitoring System (Arduino Uno Project)

This project is a complete air-quality monitoring system built on the ATmega328P (Arduino Uno).  
It measures temperature, humidity, CO₂/air quality, PM2.5, and PM10 levels and displays the results on an OLED I2C display, including a simple animated UI.

---

## Main Project Photo
air_quality_pr/media/mainPic.jpg

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
![DHT11](media/pic_dht11.png)  
**Description:**  
DHT11 contains a resistive humidity element and an NTC thermistor, managed by an internal 8-bit MCU. It outputs data via a **single-wire digital protocol**.  
Provides basic temperature and humidity measurements.
**Advantages:**
- Very low price  
- Easy to connect  
- Low power consumption  
- Internally calibrated digital output  
**Disadvantages:**
- Low accuracy (±5%RH, ±2°C)  
- Low resolution (1°C, 1%RH)  
- Slow response  
**Interface used:**  
**Single-wire digital**, connected to Arduino digital pin.

- **MQ135 – Air Quality / CO₂ Approximation Sensor**  
![MQ135](media/pic_mq135.png)
MQ-135 uses a heated SnO₂ sensitive layer whose resistance changes depending on the concentration of gases such as **NH₃, NOx, benzene, alcohol, smoke, and CO₂**.  
**It does NOT measure true CO₂ concentration in ppm** — only an approximation of "air quality".  
It also consumes a lot of power due to its internal heater (~800 mW).
**Advantages:**
- Very inexpensive  
- Easy analog interface  
- Wide range of detectable gases  

**Disadvantages:**
- Does *not* measure CO₂ directly  
- Very power-hungry (heater required)  
- Requires long preheating (24 hours for calibration)  
- Output highly unstable and environment-dependent  
- Poor long-term accuracy  

**Interface used:**  
**Analog signal** → Arduino **A1**. 
- **SDS018 – Laser Dust Sensor (PM2.5 / PM10)**
 ![SDS018](media/pic_sds018.png)  
 **Description:**  
SDS018 uses a **laser scattering principle**: particles passing through a laser beam reflect light toward a photodiode, producing pulses proportional to particle size and quantity.  
It includes an internal **fan** to ensure stable airflow.  
Outputs **PM2.5 and PM10 in μg/m³** through UART every second.  
Source: SDS018 datasheet :contentReference[oaicite:2]{index=2}

**Advantages over Sharp GP2Y1010AU0F:**
- True **laser-based** measurement (Sharp uses IR LED → less accurate)  
- Higher accuracy and repeatability  
- Built-in fan ensures consistent airflow  
- Provides digital PM values directly via UART  
- Resolution down to **0.3 μm particles**  

**Disadvantages:**
- Larger physical size  
- Fan produces some noise  
- Limited laser lifetime (~8000 hours)  

**Interface used:**  
**UART 9600 baud**  
- SDS018 TX → Arduino RX  

### Other Components
## OLED SSD1306 (128×64, I²C)
![OLED](media/pic_oled.png)

**Description:**  
A monochrome 128×64 pixel OLED display driven by the SSD1306 controller. Used to show sensor readings and air quality animations.

**Advantages:**
- Very easy to connect (only 2 data lines)  
- Low power consumption  
- High contrast and readability  
- Supports graphics and custom UI  

**Disadvantages:**
- Small size  
- Monochrome  
- OLED organic degradation over time  

**Interface used:**  
**I²C**  
- SDA → Arduino A4  
- SCL → Arduino A5  

---  
- **Arduino Uno (ATmega328P)** — system controller  
- **Breadboard, jumper wires, USB cable** — assembly and power  

### Why these components  
Each sensor provides a different environmental parameter:  
DHT11 handles temperature/humidity, MQ135 handles gases/CO₂ approximation, and SDS018 measures PM2.5/PM10 dust.  
Together they form a complete, affordable, and reliable indoor air monitoring system.

---

## Wiring Diagram
![Wiring Diagram](media/shema.jpg)

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
## Future Improvements

- **Upgrade the temperature & humidity sensor (replace DHT11 with DHT22)**  
  The DHT22 provides noticeably better accuracy and resolution (±0.5°C vs ±2°C for DHT11, and humidity accuracy of ±2–5% vs ±5–10%).  
  It also supports a wider measurement range, making the collected data more reliable.

- **Create a custom 3D-printed enclosure and a dedicated PCB**  
  A proper case would protect the electronics, improve airflow for sensors, and make the device look like a finished product.  
  Designing a PCB would remove the need for a breadboard, increase durability, and reduce electrical noise.

- **Use a more advanced CO₂ sensor (e.g., Sensirion STC31 or SCD4x series)**  
  These sensors measure actual CO₂ concentration in ppm rather than estimating "air quality" like MQ135.  
  This upgrade would dramatically improve accuracy and make the project suitable for real environmental monitoring.

---

## References

- **DHT11 Sensor Datasheet**  
  https://botland.cz/index.php?controller=attachment&id_attachment=251

- **MQ135 Gas Sensor Datasheet**  
  https://www.olimex.com/Products/Components/Sensors/Gas/SNS-MQ135/resources/SNS-MQ135.pdf

- **SDS018 Laser Dust Sensor Manual**  
  https://botland.cz/index.php?controller=attachment&id_attachment=1887

- **SSD1306 OLED Display Controller Datasheet**  
  https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf

- **ATmega328P Microcontroller Datasheet**  
  https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

- **Project Laboratory Materials (BUT FEEC – AVR Labs)**  
  https://github.com/tomas-fryza/avr-labs/tree/master/lab8-project
 

