# 📡 ESP32 Radar Visualization System

An embedded system for scanning and visualizing distances using an ultrasonic sensor and a servo-mounted radar interface displayed on a TFT screen.

## ✨ Features

- 🔄 **Servo-controlled Scanning**  
  Rotates an ultrasonic sensor (HC-SR04) from 0° to 180° using a micro servo (SG90) to detect surrounding objects.

- 📏 **Accurate Distance Measurement**  
  Measures object distances using ultrasonic pulses and converts them into centimeters with filtering for invalid readings.

- 🖥️ **Real-Time Radar Visualization**  
  Displays live radar sweeps on a TFT screen using polar-to-Cartesian conversion and real-time drawing of arcs, lines, and points.

- 📐 **Live Angle and Distance Readout**  
  Shows current scanning angle and measured distance in real time during each sweep.

- 🧭 **Graphical Radar Interface**  
  Includes classic radar arcs and radial lines for enhanced readability and aesthetic feedback.

## 🧰 Hardware Requirements

- ESP32 development board  
- HC-SR04 ultrasonic distance sensor  
- SG90 servo motor  
- 2.4” TFT display (ILI9341 or compatible, using TFT_eSPI library)  
- Jumper wires and breadboard

## 📦 Libraries Used

- [`ESP32Servo`](https://github.com/madhephaestus/ESP32Servo) – for servo control  
- [`TFT_eSPI`](https://github.com/Bodmer/TFT_eSPI) – for TFT graphics rendering
