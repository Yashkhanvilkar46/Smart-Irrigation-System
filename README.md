# 🌱 Smart Irrigation System Using IoT

An IoT-based automated irrigation system that monitors soil moisture, temperature, and humidity in real time and controls a water pump automatically — reducing water wastage and manual effort for small and medium-scale farmers.

> Community Engagement Project 2 · Semester IV · Department of Electronics & Telecommunication Engineering, Don Bosco Institute of Technology, Mumbai · AY 2025–2026
> 🏆 2nd Place, Sankalpana 2026

---

## 📌 Overview

Traditional irrigation relies on manual checking or fixed schedules, leading to over-watering, water wastage, and inefficient crop management. This project automates irrigation using real-time soil and environmental sensing, so water is supplied only when the field actually needs it.

**Relevant SDGs:** SDG 6 (Clean Water & Sanitation) · SDG 2 (Zero Hunger) · SDG 12 (Responsible Consumption & Production)

## ⚙️ How It Works

```
Soil Moisture Sensor (Zone 1) ─┐
Soil Moisture Sensor (Zone 2) ─┼──► ESP32 ──► Relay Module ──► Water Pump
DHT11 (Temp & Humidity)       ─┘       │
                                        ▼
                              Wi-Fi ──► Blynk IoT App
                                        │
                              Local Web Dashboard
```

1. Two soil moisture sensors (start and end of field) continuously monitor moisture levels.
2. The DHT11 sensor tracks ambient temperature and humidity.
3. The ESP32 compares readings against a predefined threshold.
4. If soil is dry, the relay switches the water pump ON; once moisture is sufficient, it switches OFF automatically.
5. Live readings and pump status are pushed to the **Blynk IoT app** and a **local web dashboard** hosted directly on the ESP32.

## 🔩 Hardware Components

| Component | Purpose |
|---|---|
| ESP32 | Main microcontroller (Wi-Fi + processing) |
| Soil Moisture Sensor ×2 | Measures soil water content at two field zones |
| DHT11 Sensor | Measures temperature & humidity |
| IR Sensor | Water flow / object detection |
| Relay Module | Switches the pump ON/OFF |
| DC Water Pump | Delivers water to the field |

## 💻 Software & Tools

- Arduino IDE (Embedded C/C++)
- Blynk IoT Platform (mobile dashboard)
- ESP32 local web server (offline dashboard)

## 📊 Results

- Water conservation improved significantly by avoiding over-irrigation
- Automatic pump control validated across dry, moderate, and saturated soil conditions
- Real-time monitoring confirmed on both Blynk app and local dashboard (no internet needed for local view)

| Soil Moisture | Pump Status | Condition |
|---|---|---|
| 20% | ON | Dry Soil |
| 35% | ON | Moderate Moisture |
| 60% | OFF | Sufficient Moisture |
| 80% | OFF | Wet Soil |

## 🗺️ Project Roadmap

| Week | Phase |
|---|---|
| 1 | Planning & Research |
| 2 | Hardware Development |
| 3 | IoT Integration |
| 4 | Testing & Optimization |
| 5 | Documentation & Presentation |

## 📁 Repository Structure

```
smart-irrigation-system/
├── README.md
├── docs/
│   └── Smart_Irrigation_Report.pdf
├── firmware/
│   └── smart_irrigation.ino
└── images/
    ├── block_diagram.png
    ├── hardware_setup.jpg
    ├── blynk_dashboard.jpg
    └── local_dashboard.jpg
```

## 👥 Team

- Yash Khanvilkar
- Rohit Kumar
- Harshit Patil
- Tirth Shetty

**Supervised by:** Ms. Freda Carvalho, Assistant Professor, EXTC

## 📄 Full Report

See [`docs/Smart_Irrigation_Report.pdf`](https://github.com/Yashkhanvilkar46/Smart-Irrigation-System/blob/main/Smart_Irrigation_Report.pdf) for complete literature survey, methodology, and datasheets.

## 🚀 Future Scope

- AI-based irrigation scheduling
- Weather forecast integration
- GSM-based alerts for no-internet areas
- Solar-powered backup for remote fields

---
*Built as part of the IEEE-DBIT community engagement initiative.*
