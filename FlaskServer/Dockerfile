# Use Python 3.10 as the base image
FROM python:3.10-slim

RUN apt update && apt install -y sqlite3

# Set the working directory inside the container
WORKDIR /app

# Copy the requirements.txt file and install dependencies
COPY FlaskServer/requirements.txt /app/
RUN pip install --no-cache-dir -r /app/requirements.txt

# Copy the entire Flask app into the container
COPY FlaskServer /app/

# Copy the db directory into /app/db/
COPY FlaskServer/db /app/db/

# Set permissions for the db folder and file
RUN chmod -R 777 /app/db/

# Expose the Flask app's port
EXPOSE 5000

# Command to run the Flask app
CMD ["python", "app.py"]
