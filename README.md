# ESP32 Soil Monitoring System

This project involves an ESP32 connected to various sensors to monitor environmental parameters like temperature, humidity, and TDS (Total Dissolved Solids) of a soil. The data is published via MQTT and stored in a backend server using Flask. This project aims to collect sensor data, process it, and visualize the results.

## Components Used:
- **ESP32**: A low-power microcontroller with Wi-Fi and Bluetooth capabilities.
- **1-Wire Humidity Sensor and Temperature Sensor (SHT31, DS18B20)**: A digital temperature and humidity sensor connected through the 1-wire protocol.
- **TDS Sensor(SEN0244)**: Measures the Total Dissolved Solids (TDS) concentration in a soil.

   
### 1. **Software:**
   - **Arduino IDE**:
     - Install the ESP32 board package.
     - Install necessary libraries:
       - `DHT` (for DHT11/DHT22)
       - `OneWire` and `DallasTemperature` (for DS18B20)

### 2. **MQTT Broker:**
   - This project uses MQTT to send sensor data to a broker. Set up your MQTT broker (Mosquitto) on your local machine or use a cloud service like [Adafruit IO](https://io.adafruit.com/).

### 3. **Flask Backend:**
   - The data is processed and stored in a backend server using Flask.
   - Flask listens for incoming MQTT messages and saves sensor data to a database (SQLite).
