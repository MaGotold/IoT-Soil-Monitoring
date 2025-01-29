#include "mqtt_config.h"
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* mqtt_server = "";  
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 0, 60000);

//connect to wifi
void connectWiFi() {
    WiFi.begin("", "");  
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());  
}

//connect to mqtt
void reconnect() {
    while (!client.connected()) {
        Serial.println("Attempting MQTT connection...");
        String clientId = "ESP32Client-";
        clientId += String(WiFi.macAddress());

        if (client.connect(clientId.c_str())) {
            Serial.println("Connected to MQTT broker");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            delay(5000);
        }
    }
}


void mqttCallback(char* topic, byte* payload, unsigned int length) {
    // Handle incoming messages if needed
}

void setupMQTT() {
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(mqttCallback);
}

void publishData(float temp, float hum, float tds) {
    StaticJsonDocument<200> doc;

    //get date time 
    timeClient.update();    
    unsigned long epochTime = timeClient.getEpochTime();
    time_t rawTime = (time_t)epochTime;
    struct tm *timeInfo = localtime(&rawTime);
    char dateBuffer[20]; 
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    String formattedDateTime = String(dateBuffer);  

    //data to send
    doc["temperature"] = temp;
    doc["humidity"] = hum;
    doc["tds"] = tds;
    doc["timestamp"] = formattedDateTime;
    Serial.println(formattedDateTime);

    //serialize to json
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);

    client.publish("sensor_data", jsonBuffer);  
}
