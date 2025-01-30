import os
import sqlite3
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from datetime import datetime

# Database path
DB_NAME = os.path.join(os.getcwd(), 'db', 'sensor_data.db')

# Ensure static directory exists
STATIC_DIR = os.path.join(os.getcwd(), 'static')
os.makedirs(STATIC_DIR, exist_ok=True)

# Connect to database
conn = sqlite3.connect(DB_NAME)
cursor = conn.cursor()

# Fetch data
cursor.execute("SELECT timestamp, temperature, humidity, el_conductivity FROM sensor_data ORDER BY timestamp ASC")
data = cursor.fetchall()
conn.close()

if not data:
    print("No data found in the database.")
    exit()

# Process data
timestamps = [datetime.strptime(row[0], '%Y-%m-%d %H:%M:%S') for row in data]
temperatures = [row[1] for row in data]
tds_values = [row[3] for row in data]

# Get latest soil condition
latest_humidity = data[-1][2]  # Last humidity value
soil_condition = "Wet" if latest_humidity == 1 else "Dry"

# Save soil condition as a text file
soil_condition_path = os.path.join(STATIC_DIR, 'soil_condition.txt')
with open(soil_condition_path, 'w') as f:
    f.write(soil_condition)

# Plot setup
fig, axs = plt.subplots(2, 1, figsize=(10, 6), sharex=True)

# Temperature Graph
axs[0].plot(timestamps, temperatures, marker='o', linestyle='-', color='r', label='Teplota (°C)')
axs[0].set_ylabel('Teplota (°C)')
axs[0].legend()
axs[0].grid()

# TDS Graph
axs[1].plot(timestamps, tds_values, marker='s', linestyle='-', color='g', label='Koncentrácia rozpustených látok (mg/L)')
axs[1].set_ylabel('Koncentrácia rozpustených látok (mg/L)')
axs[1].legend()
axs[1].grid()

# Format x-axis
axs[1].set_xlabel('Čas')
axs[1].xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d %H:%M'))
plt.xticks(rotation=45)

# Save plot
output_path = os.path.join(STATIC_DIR, 'dashboard_plot.png')
plt.tight_layout()
plt.savefig(output_path)
plt.close()

print(f"Plot saved at: {output_path}")
print(f"Soil condition saved at: {soil_condition_path}")
