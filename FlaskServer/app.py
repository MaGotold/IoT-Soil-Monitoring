from flask import Flask, render_template
from mqtt_client import connect_mqtt
from db import create_table
from visualization import create_plot

app = Flask(__name__)


@app.route('/plot.png') 
def plot_png():
    return create_plot()

@app.route("/")
def home():
    return render_template('sensor_data.html')


connect_mqtt()


if __name__ == "__main__":
    print("hello")
    create_table()
   
    app.run(host="0.0.0.0", port=5000, debug = True)