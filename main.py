import requests
import pandas as pd

# ThingSpeak Channel Details
CHANNEL_ID = "2801217"
READ_API_KEY = "6S5SJ75P959H270U"
RESULTS = 10000 # Number of data points to fetch

# API URL
url = f"https://api.thingspeak.com/channels/{CHANNEL_ID}/feeds.json?api_key={READ_API_KEY}&results={RESULTS}"

# Fetch Data
response = requests.get(url)
data = response.json()

# Extract Fields
timestamps = [entry["created_at"] for entry in data["feeds"]]
temperature = [entry["field1"] for entry in data["feeds"]]
humidity = [entry["field2"] for entry in data["feeds"]]
pm25 = [entry["field3"] for entry in data["feeds"]]
pm10 = [entry["field4"] for entry in data["feeds"]]

# Create DataFrame
df = pd.DataFrame({
    "Timestamp": timestamps,
    "Temperature (°C)": temperature,
    "Humidity (%)": humidity,
    "PM2.5 (µg/m³)": pm25,
    "PM10 (µg/m³)": pm10
})

# Save as CSV
df.to_csv("thingspeak_data.csv", index=False)
print("Data saved successfully as thingspeak_data.csv")
