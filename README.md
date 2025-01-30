
# ESP32 Soil Monitoring System

This project involves an ESP32 connected to various sensors to monitor environmental parameters like temperature, humidity, and TDS (Total Dissolved Solids) of a soil. The data is published via MQTT and stored in a backend server using Flask. Additionally, the server provides a real-time visualization of the measured data for the last 24 hours. This project is containerized using Docker for easy deployment.

## Components Used:
- **ESP32**: A low-power microcontroller with Wi-Fi and Bluetooth capabilities.
- **1-Wire Humidity Sensor and Temperature Sensor (SHT31, DS18B20)**: A digital temperature and humidity sensor connected through the 1-wire protocol.
- **TDS Sensor (SEN0244)**: Measures the Total Dissolved Solids (TDS) concentration in soil.

### 1. **Software:**
   - **Arduino IDE**:
     - Install the ESP32 board package.
     - Install necessary libraries:
       - `DHT` (for DHT11/DHT22)
       - `OneWire` and `DallasTemperature` (for DS18B20)

### 2. **MQTT Broker:**
   This project uses MQTT to send sensor data to a broker. The MQTT broker (Mosquitto) is containerized and runs within a Docker container. You don't need to install it on your local machine. You can configure and run it directly from Docker for easier deployment. Alternatively, you can also use a cloud service like Adafruit IO.

### 3. **Flask Backend:**
   - The data is processed and stored in a backend server using Flask.
   - Flask listens for incoming MQTT messages and saves sensor data to a database (SQLite).
   - The server is **containerized** using **Docker**, simplifying deployment and ensuring consistency across environments.

### 4. **Data Visualization:**
   - The backend server provides a web interface to visualize the measured data from the last 24 hours.
   - It displays graphs for temperature, and TDS values also showing weather soil is wet or dry, offering real-time insights into the soil conditions.



### Quick Setup

1. **Clone the repository**:
   ```bash
   git clone https://github.com/MaGotold/IoT-Soil-Monitoring.git
   ```

2. **Build and run the application with Docker Compose**:
   In the project directory, run the following command to build and start all services (Flask backend, MQTT broker, and any other dependencies):
   ```bash
   docker-compose up --build
   ```

   This will:
   - Build the Docker images.
   - Start the Flask backend, MQTT broker, and any other necessary services as defined in the `docker-compose.yml`.

3. **Access the application**:
   Once the containers are up and running, you can access the visualization at:
   ```
   http://localhost:5000/dashboard
   ```

   This will show the dashboard with the latest sensor data.

4. **Shut down the application**:
   To stop the application, run:
   ```bash
   docker-compose down
   ```

### 5. **Hardware Setup:**
   - Connect the necessary sensors (temperature, humidity, TDS) to the ESP32. Note: for application to run you don't need all sensors mentioned.
   - Ensure the ESP32 is connected to Wi-Fi.
   - Connect the ESP32 to MQTT broker running inside Docker.
   - Once everything is set up and the hardware is connected, you can see the measured and visualized data at http://localhost:5000/dashboard.


