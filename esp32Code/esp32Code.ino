#include <WiFi.h>
//#include "Wifi_setup.h"  // Uncomment if you have a WiFi setup file

#include "mqtt_config.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <esp_sleep.h>  // For deep sleep functionality

// Define GPIO pins
#define ONE_WIRE_BUS 5     // GPIO 17 for DS18B20 sensor
#define TDS_PIN 34         // GPIO 34 for TDS sensor
#define SOIL_SENSOR_PIN 12 // GPIO 12 for the digital soil moisture sensor (DO)

// Declare the objects
OneWire oneWire(ONE_WIRE_BUS);        // OneWire for DS18B20
DallasTemperature sensors(&oneWire);  // DallasTemperature for DS18B20

void setup() {
    Serial.begin(115200);  
    delay(10);
    pinMode(SOIL_SENSOR_PIN, INPUT_PULLUP);  // Set the soil sensor pin as input

    // Connect to Wi-Fi and MQTT
    connectWiFi();  // Assuming you have this function for Wi-Fi connection
    setupMQTT();    // Assuming you have this function for MQTT setup

    sensors.begin();  // Initialize DS18B20 sensor

    // Set up the deep sleep timer to wake up after 10 seconds
    esp_sleep_enable_timer_wakeup(10 * 1000000);  // 10 seconds in microseconds

    // Check if we're waking up from deep sleep or if it's the initial boot
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
        Serial.println("Woke up from deep sleep!");
    }
    else {
        Serial.println("Fresh boot!");
    }
}

void loop() {
    // Only run this part if the ESP32 is awake
    Serial.println("Waking up...");

    // Reconnect to Wi-Fi if it's not connected
    if (!WiFi.isConnected()) {
        Serial.println("Reconnecting to Wi-Fi...");
        connectWiFi();
    }
    
    // Reconnect to MQTT if it's not connected
    if (!client.connected()) {
        Serial.println("Reconnecting to MQTT...");
        reconnect();  // Ensure MQTT is working
    }
    client.loop();  // Keep MQTT connection alive

    // Read Temperature - DS18B20
    sensors.requestTemperatures();  // Request temperature reading
    float temperature = sensors.getTempCByIndex(0);  // Get temperature in °C
    if (temperature == DEVICE_DISCONNECTED_C) {
        Serial.println("Error: Could not read temperature.");
    } else {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" °C");
    }

    // Read TDS - TDS Sensor
    int tdsValue = analogRead(TDS_PIN);  // Read analog value from TDS sensor
    float tdsVoltage = (tdsValue / 1023.0) * 3.3;  // Convert to voltage
    float tds = tdsVoltage * 1000;  // Convert to TDS value in ppm (adjust if needed)
    if (tds < 0) {
        tds = 0;  // Prevent negative values
    }

    Serial.print("TDS Value: ");
    Serial.print(tds);
    Serial.println(" ppm");

    // Read Soil Moisture - Digital soil sensor (DO)
    int soilMoistureValue = digitalRead(SOIL_SENSOR_PIN);  // Read the soil moisture sensor (DO)
    Serial.println(soilMoistureValue);  // Print the actual value of soilMoistureValue

    if (soilMoistureValue == 0) {
        Serial.println("Soil is dry.");
    } else {
        Serial.println("Soil is wet.");
    }

    // Publish data to MQTT broker
    publishData(temperature, soilMoistureValue == 1 ? 0 : 1, tds);  // Publish 100 for wet, 0 for dry soil

    // Now enter deep sleep for 10 seconds before waking up again
    Serial.println("Entering deep sleep for 10 seconds...");
    delay(1000);  // Give time for the message to be printed to the serial console
    esp_deep_sleep_start();  // Enter deep sleep mode
}
