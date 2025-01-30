#include <WiFi.h>
#include "mqtt_config.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <esp_sleep.h>  

// Define GPIO pins
#define ONE_WIRE_BUS 5     
#define TDS_PIN 34         
#define SOIL_SENSOR_PIN 12 

OneWire oneWire(ONE_WIRE_BUS);        
DallasTemperature sensors(&oneWire);  

void setup() {
    Serial.begin(115200);  
    delay(10);
    pinMode(SOIL_SENSOR_PIN, INPUT_PULLUP);  

    // Connect to Wi-Fi and MQTT
    connectWiFi();  
    setupMQTT();    

    sensors.begin();

    esp_sleep_enable_timer_wakeup(30 * 60 * 1000000);
    
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
        Serial.println("Woke up from deep sleep!");
    }
    else {
        Serial.println("Fresh boot!");
    }
}

void loop() {
    Serial.println("Waking up...");

    // Reconnect to Wi-Fi if it's not connected
    if (!WiFi.isConnected()) {
        Serial.println("Reconnecting to Wi-Fi...");
        connectWiFi();
    }
    
    // Reconnect to MQTT if it's not connected
    if (!client.connected()) {
        Serial.println("Reconnecting to MQTT...");
        reconnect();  
    }
    client.loop();  

    // Temp
    sensors.requestTemperatures();  
    float temperature = sensors.getTempCByIndex(0);  
    if (temperature == DEVICE_DISCONNECTED_C) {
        Serial.println("Error: Could not read temperature.");
    } else {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" °C");
    }

    //TDS
    int tdsValue = analogRead(TDS_PIN);  
    float tdsVoltage = (tdsValue / 1023.0) * 3.3;  
    float tds = tdsVoltage * 1000;  
    if (tds < 0) {
        tds = 0;  
    }

    Serial.print("TDS Value: ");
    Serial.print(tds);
    Serial.println(" ppm");

    //hum
    int soilMoistureValue = digitalRead(SOIL_SENSOR_PIN);  
    Serial.println(soilMoistureValue);  

    if (soilMoistureValue == 0) {
        Serial.println("Soil is dry.");
    } else {
        Serial.println("Soil is wet.");
    }

    publishData(temperature, soilMoistureValue == 1 ? 0 : 1, tds);  

    Serial.println("Entering deep sleep for 10 seconds...");
    delay(1000);  
    esp_deep_sleep_start();  
}
