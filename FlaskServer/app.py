from flask import Flask, render_template, send_file
from mqtt_client import connect_mqtt
from db import create_table
import subprocess
import os


app = Flask(__name__)

# Define static directory
STATIC_DIR = os.path.join(os.getcwd(), 'static')
os.makedirs(STATIC_DIR, exist_ok=True)


@app.route('/dashboard')
def dashboard():
    # Generate the latest visualization
    script_path = os.path.join(os.getcwd(), 'visualization.py')
    subprocess.run(["python", script_path], check=True)

    # Read soil condition
    soil_condition_path = os.path.join(STATIC_DIR, 'soil_condition.txt')
    if os.path.exists(soil_condition_path):
        with open(soil_condition_path, 'r') as f:
            soil_status = f.read().strip()
    else:
        soil_status = "Unknown"

    return render_template('dashboard.html', soil_status=soil_status)

@app.route('/static/dashboard_plot.png')
def get_plot():
    return send_file(os.path.join(STATIC_DIR, 'dashboard_plot.png'), mimetype='image/png')

connect_mqtt()


if __name__ == "__main__":
    
    create_table()
    app.run(host="0.0.0.0", port=5000, debug = True)