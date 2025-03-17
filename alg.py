import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

# Load dataset
data_path = r"D:\IIITH\Workshop\collect\thingspeak_data.csv"  # Update if needed
df = pd.read_csv(data_path)

# Convert 'created_at' to datetime format
df['created_at'] = pd.to_datetime(df['created_at'])
df.set_index('created_at', inplace=True)

# Resample data hourly and compute the mean values
hourly_avg = df.resample('h').mean()

# Define time categories
morning = (hourly_avg.index.hour >= 6) & (hourly_avg.index.hour < 12)
afternoon = (hourly_avg.index.hour >= 12) & (hourly_avg.index.hour < 18)
evening = (hourly_avg.index.hour >= 18) & (hourly_avg.index.hour < 24)
night = (hourly_avg.index.hour >= 0) & (hourly_avg.index.hour < 6)

# Assign categories to data
hourly_avg['Time_of_Day'] = np.select([morning, afternoon, evening, night],
                                      ['Morning', 'Afternoon', 'Evening', 'Night'],
                                      default='Unknown')

# Compute average values for each time of day
time_of_day_avg = hourly_avg.groupby('Time_of_Day')[['Temperature', 'Humidity', 'PM2.5', 'PM10']].mean()
print(time_of_day_avg)

# Plot the results
plt.figure(figsize=(10, 6))
time_of_day_avg.plot(kind='bar', figsize=(12, 6), colormap='viridis')
plt.title("Average Air Quality Parameters by Time of Day")
plt.ylabel("Average Values")
plt.xlabel("Time of Day")
plt.xticks(rotation=45)
plt.legend(loc='upper right')
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.show()
