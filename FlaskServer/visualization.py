import matplotlib.pyplot as plt
import io
from datetime import datetime
import sqlite3
from flask import Response
import matplotlib
matplotlib.use('Agg')
import matplotlib.dates as mdates

def fetch_data():
    conn = sqlite3.connect('sensor_data.db')  
    cursor = conn.cursor()
    query = """
    SELECT timestamp, temperature, humidity, el_conductivity
    FROM sensor_data
    ORDER BY timestamp ASC
    """
    cursor.execute(query)
    data = cursor.fetchall()
    conn.close()
    return data

def create_plot():
    data = fetch_data()
    if not data:
        return "No data available to plot.", 404

    timestamps, temperatures, humidities, conductivities = zip(*data)
    timestamps = [datetime.strptime(ts, '%Y-%m-%d %H:%M:%S') for ts in timestamps]

    # Create individual plots without sharex
    fig, axs = plt.subplots(3, 1, figsize=(10, 15), sharex =False)

    # Plot temperature
    axs[0].plot(timestamps, temperatures, label='Teplota (°C)', color='red')
    axs[0].set_title("Teplota")
    axs[0].set_ylabel("Teplota (°C)")
    axs[0].grid(True)
    axs[0].legend()
    axs[0].xaxis.set_major_locator(mdates.AutoDateLocator())
    axs[0].xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d %H:%M:%S'))
    axs[0].tick_params(axis='x', rotation=45)
    axs[0].set_xlabel('Čas', fontsize=12)  # Adding x-axis label for this subplot

    # Plot humidity
    axs[1].plot(timestamps, humidities, label='Vlhkosť (%)', color='blue')
    axs[1].set_title("Vlhkosť")
    axs[1].set_ylabel("Vlhkosť (%)")
    axs[1].grid(True)
    axs[1].legend()
    axs[1].xaxis.set_major_locator(mdates.AutoDateLocator())
    axs[1].xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d %H:%M:%S'))
    axs[1].tick_params(axis='x', rotation=45)
    axs[1].set_xlabel('Čas', fontsize=12)  # Adding x-axis label for this subplot

    # Plot electrical conductivity
    axs[2].plot(timestamps, conductivities, label='Elektrická vodivosť (µS/cm)', color='green')
    axs[2].set_title("Elektrická vodivosť")
    axs[2].set_xlabel("Čas")
    axs[2].set_ylabel("Elektrická vodivosť (µS/cm)")
    axs[2].grid(True)
    axs[2].legend()
    axs[2].xaxis.set_major_locator(mdates.AutoDateLocator())
    axs[2].xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d %H:%M:%S'))
    axs[2].tick_params(axis='x', rotation=45)
    axs[2].set_xlabel('Čas', fontsize=12)  # Adding x-axis label for this subplot

    # Rotate date labels for better visibility
    fig.autofmt_xdate()

    # Save the plot to a BytesIO object for displaying in Flask
    img = io.BytesIO()
    plt.savefig(img, format='png')
    img.seek(0)
    plt.close(fig)
    return Response(img.getvalue(), mimetype='image/png')
