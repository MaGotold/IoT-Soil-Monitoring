# Use Python 3.8 as the base image
FROM python:3.10-slim

RUN apt update && apt install -y sqlite3

# Set the working directory inside the container
WORKDIR /app


# Copy requirements.txt and install dependencies
#COPY requirements.txt /app/
RUN pip install --no-cache-dir -r requirements.txt

# Copy the entire Flask app into the container
COPY FlaskServer /app/

# Set permissions for the db folder and file (make sure the app can read/write)
RUN chmod -R 777 /app/db

# Expose the Flask app's port (adjust if necessary)
EXPOSE 5000

# Command to run the Flask app
CMD ["python", "app.py"]

