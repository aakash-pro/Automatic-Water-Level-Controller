# 💧 Smart Water Pump & Tank Management System.

A fully automated, WiFi-enabled **Smart Water Tank Management System**
built using **NodeMCU (ESP8266)** and **Tasmota Based Smart Plug**.\
This system intelligently monitors tank levels, protects the pump from
dry-run damage,overload damage, Prevents tank overflow , and provides a powerful web-based
dashboard for real-time monitoring and pump scheduling.

------------------------------------------------------------------------

## 🚀 Project Overview

This project automates water pump control using 8 discrete level sensors
and a flow sensor.\
It provides real-time monitoring through:

-   128x64 LCD display\
-   WS2812 8-Led RGB Strip for tank level\
-   Local web dashboard\
-   Linux-Cron based scheduling

Designed for residential buildings, apartments, farms, and smart homes.

------------------------------------------------------------------------

## 🧠 System Architecture

### 🔹 Controller Unit

-   NodeMCU (ESP8266 based)
-   128x64 SPI LCD
-   8-bit WS2812 RGB LED Strip

### 🔹 Pump Control Unit

-   Tasmota-based Smart Plug (Relay based) with power monitoring capability.

### Tank Monitoring Unit

- 8 water level sensors ( Reed switch based ) 
- Flow sensor YF-S201 
- Flow & water level sensors connected to NODE MCU running tasmota.
------------------------------------------------------------------------

## ✨ Features

### 🌊 Smart Water Level Monitoring

-   8-point tank level detection
-   Real-time percentage display
-   LED visual indication (color coded)
-   LCD status updates

### 🛡️ Tank Full Protection

-   Automatically stops pump at your desired tank level.
-   Prevents overflow and water wastage
-   Tracks & monitors water flow in LPM

### 🚱 Pump Dry Run Protection

-   Flow sensor validation
-   Pump power consumption Validation.
-   If no water flow detected → Pump shuts down
-   If Pumps consumes less power then desired -> Pump Shut down.

### 📅 Powerfull linux-cron based Pump scheduling.

-   User friendly scheduler on web dashboard.
-   hourly , daily , weekly, alternate days , monthly schedules.


### 📶 Automatic WiFi Provisioning

-   Captive portal on first boot
-   Easy WiFi configuration
-   No hardcoded credentials

### 🌐 Web-Based User Interface

Accessible via browser:

    http://device-ip

Includes 4 Custom LCD Dashboards: -
📊 Real-Time Monitoring 
- ⚙️ Pump Stats
- 📅 Tank stats 
- 📈 Flow Analytics

Fully Local Control --- No Cloud Dependency.

________________________________________________________________________

## 🔗 Integration Support

-   MQTT Compatible
-   HTTP API Support
-   Works with Home Automation Platforms
-   Can integrate with Home Assistant

## 📜 License

This project is open-source.\
Contributions, feature requests, and pull requests are welcome.

------------------------------------------------------------------------

## 👨‍💻 Author : aakash-pro

Developed with ❤️ for smart and efficient water management.
