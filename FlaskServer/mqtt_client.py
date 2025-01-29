import paho.mqtt.client as mqtt
from esp32_1 import on_message
import os


# MQTT config
BROKER_HOST = os.getenv("MQTT_BROKER_HOST", "mqtt-broker")  # Default to "mqtt-broker" if not set
BROKER_PORT = 1883

# create new client
mqtt_client = mqtt.Client()

is_subscribed = False
def on_connect(client, userdata, flags, rc):
    global is_subscribed
    if rc == 0:
        print("Successfully connected")
        if not is_subscribed:
            subscribe_topic("sensor_data")
            is_subscribed = True
    else: print(f"Failed to connect, return code {rc}")
    


def connect_mqtt():
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message  
    mqtt_client.connect(BROKER_HOST, BROKER_PORT, 60)
    mqtt_client.loop_start()  
    

def subscribe_topic(topic):
    mqtt_client.subscribe(topic, qos = 0)
    
    
def publish_data(topic, message):
    mqtt_client.publish(topic, message)
    
