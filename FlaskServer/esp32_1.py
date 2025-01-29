from flask import Blueprint
import json
from db import insert_data


esp32_1 = Blueprint("esp32_1", __name__)


def on_message(client, userdata, msg):
    
    try:
        data = json.loads(msg.payload.decode("utf-8"))
        print(data)
        
        # Extract sensor data
        temp = data.get("temperature")
        hum = data.get("humidity")
        tds = data.get("tds")
        timestamp = data.get("timestamp")
        
        if None in [temp, hum, tds, timestamp]:
            print("Error: Missing data")
            return

        insert_data(temp, hum, tds, timestamp)
        

        print("Data successfully processed and saved.")
    except Exception as ex:
        print(f"Error processing message: {str(ex)}")