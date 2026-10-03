# 🛠️ Smart Coal Mine Environmental Monitoring & Automated Safety System

An IoT-based real-time environmental monitoring and automated safety system designed for underground coal mines. The system monitors parameters like **Temperature, Methane Gas Levels, and Water Ingress**, triggering local safety actuators and sending telemetry data for emergency alerts.

---

## 📌 Features
- **Multi-Sensor Environmental Monitoring:** Tracks Temperature (DHT22), Methane Gas (Potentiometer Emulated), and Water Level (Potentiometer Emulated).
- **Automated Local Safety Loop:** Instantly triggers high-decibel Buzzer and Red Warning LED upon threshold breach (>45°C Temp, High Gas/Water).
- **Hybrid Communication Architecture:** Designed to bridge underground non-Wi-Fi zones with surface IoT gateways via Sub-GHz RF/ESP-NOW and Cloud Protocols (MQTT/HTTP).
- **Live Local Display:** Real-time visual metrics shown on 16x2 I2C LCD.
- **Wokwi Cloud Simulation:** Complete hardware circuit and logic emulated on ESP32-S2 platform.

---

## 🏗️ System Architecture & Connectivity

### **1. Hardware Connections (ESP32-S2)**

| Component | Component Pin | ESP32-S2 GPIO Pin | Description |
| :--- | :--- | :--- | :--- |
| **DHT22** | Data | GPIO 11 | Temperature & Humidity Sensor |
| **Gas Sensor (Potentiometer 1)** | Signal | GPIO 1 (ADC1) | Methane Gas Analog Input |
| **Water Level (Potentiometer 2)**| Signal | GPIO 2 (ADC1) | Flood/Water Ingress Analog Input |
| **LCD 1602 (I2C)** | SDA / SCL | GPIO 8 / GPIO 9 | I2C Serial Display |
| **Safety Buzzer** | Positive (+) | GPIO 12 | Local Audible Alarm |
| **Warning LED** | Anode (+) | GPIO 13 | Local Visual Alarm |

---

## 💻 Simulation Setup (Wokwi)

The project can be run directly in the **Wokwi Simulation Environment**.

### **How to Run:**
1. Open [Wokwi.com](https://wokwi.com) and create an **ESP32-S2** project.
2. Copy the contents of `simulation/diagram.json` into the `diagram.json` tab in Wokwi.
3. Copy the code from `src/sketch.ino` into the main sketch tab.
4. Click **Start Simulation**.

---

## 🚨 Disaster Defense & Failure Handling Strategy

### **1. Underground Connectivity (Wi-Fi Absence)**
- Underground nodes utilize penetration-capable **Sub-GHz RF / ESP-NOW wireless protocols** to relay sensor packets to the surface Master Controller located at the tunnel entrance.

### **2. Network Interruption & Data Caching**
- In case of cloud/Wi-Fi downtime at the surface gateway, critical telemetry payloads are cached in **ESP32 SPIFFS Flash Memory (Queue Data Caching)** and re-transmitted via **MQTT QoS 1** upon connection recovery.
- The **Local Safety Loop (Buzzer + LED)** operates independently of external network status to guarantee immediate worker notification.

---

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
