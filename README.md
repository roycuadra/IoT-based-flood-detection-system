# 🚨 IoT-Based Flood Detection System with Blynk and OLED Display

## 🎥 Video Demonstration

Check it out on TikTok: [Video Demonstration](https://www.tiktok.com/@qubits.io/video/7530842254420774162?is_from_webapp=1&sender_device=pc&web_id=7491002461630400008)

---

## 📖 Project Description

This project is an IoT-based flood detection system using an **ESP8266** microcontroller, **ultrasonic sensor**, and **Blynk IoT platform**. It measures water level distance and provides **visual alerts (via OLED & LEDs)** and **audible alerts (via buzzer)**.

### ✨ Features

- 📏 **Distance Monitoring** using HC-SR04 (in cm and inches)
- 📱 **Real-time Data** sent to Blynk using virtual pins `V1` and `V2`
- 📺 **OLED Display** for local visualization of distance
- 🔴🔵🟢 **LED Indicators** for water level thresholds
- 🔊 **Buzzer Alarm** when water level is dangerously high
- ☁️ **Cloud Logging** via Blynk `logEvent("high_water_levels")`

---

## 🧰 Hardware Components

| Component                | Description                        |
|--------------------------|------------------------------------|
| ESP8266 NodeMCU          | Main microcontroller (Wi-Fi)       |
| HC-SR04 Ultrasonic Sensor| Distance measurement               |
| OLED SSD1306 (128x64)    | Displays real-time distance data   |
| Buzzer                   | Audible alert                      |
| Red, Blue, Green LEDs    | Visual indicators for distance     |
| 220Ω Resistors (x3)      | Current-limiting for LEDs          |
| Jumper Wires             | Connections                        |
| Breadboard               | Prototyping                        |

---

## 🔌 Pin Connections

| Component      | Pin Label | ESP8266 GPIO | NodeMCU Pin |
|----------------|-----------|--------------|-------------|
| **Ultrasonic** | Trig      | GPIO12       | D6          |
|                | Echo      | GPIO14       | D5          |
| **Buzzer**     | +         | GPIO13       | D7          |
| **Red LED**    | +         | GPIO3        | D9 (RX)     |
| **Blue LED**   | +         | GPIO2        | D4          |
| **Green LED**  | +         | GPIO15       | D8          |
| **OLED**       | SDA       | GPIO4        | D2          |
|                | SCL       | GPIO5        | D1          |

---

## 🖼️ Circuit Diagram

![Circuit Diagram](image1.png)


---

## 📜 License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for full details.
