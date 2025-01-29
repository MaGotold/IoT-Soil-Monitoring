#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

#include <WiFi.h>
#include <PubSubClient.h>

// MQTT parameters
extern const char* mqtt_server;
extern const int mqtt_port;
extern WiFiClient espClient;
extern PubSubClient client;

void connectWiFi();
void reconnect();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void setupMQTT();
void publishData(float temperature, float hum, float tds);

#endif
