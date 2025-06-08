# Aquaponics System - IoT & AI Based Smart Agriculture

## 🌿 Real-World System Model

<div style="display: flex; gap: 10px;">
  <img src="./image/smart_aquaponics_system.png" alt="Real System 1" width="400"/>
  <img src="./image/iot_real_system.jpg" alt="Real System 2" width="400"/>
</div>

## 📌 Introduction

The **Aquaponics System** project aims to develop a model that integrates **smart agriculture (IoT)** and **artificial intelligence (AI)** to monitor and control a sustainable fish-vegetable ecosystem.

The system includes:

- **ESP32-S3-Relay-6CH** for collecting data from sensors (pH, DO, Temperature)
- **Industrial sensors ISHC205, ISDC205, ISEC205** to measure pH level, dissolved oxygen, and electrical conductivity in water
- **CoreIOT** for remote device control, scheduling, and threshold-based alerts
- **AI LSTM model** to forecast environmental fluctuations for proactive alerts and ecosystem protection

## 🧠 Key Features

- **Automated data collection** from sensors via Modbus RTU RS485
- **Peripheral device control** (pumps, lights, aerators...) manually or on schedule
- **Telegram alerts** when values exceed safe thresholds
- **Over-the-air (OTA) firmware updates**
- **GPS location and Wi-Fi signal strength display** on the map
- **AI LSTM prediction** for pH, DO, and Temperature in the next 15 minutes

## 🤖 AI Prediction & Early Warning

The AI model is trained using a Conv1D + BiLSTM + Attention architecture based on historical sensor data. After deployment on a Flask-SocketIO server, the processing pipeline is as follows:

1. ESP32 sends data every 5 minutes  
2. Server collects and normalizes the data  
3. Predicts three environmental parameters  
4. Sends prediction results to the dashboard and ESP32  
5. Triggers alerts if values are forecast to exceed safety thresholds  

### 📈 Sample Prediction Results

| Parameter | MAE   | RMSE  | R²     |
|-----------|-------|-------|--------|
| pH        | 0.020 | 0.027 | 0.712  |
| DO        | 0.200 | 0.260 | -2.11  |
| Temp      | 0.041 | 0.054 | 0.987  |

![AI Prediction Sample](./image/AI_prediction_result.png)

## 🖥️ Dashboard Interface

- Real-time data monitoring  
- Intuitive alert board  
- AI prediction charts  
- Manual/scheduled device control  
- GPS + RSSI visualization on map  

## 🧰 Technologies Used

- ESP32-S3 + RS485 Sensors  
- Flask + Socket.IO + Eventlet  
- CoreIOT (ThingsBoard)  
- Keras/TensorFlow + joblib  
- HTML/CSS/JS Dashboard  
- Telegram API  
