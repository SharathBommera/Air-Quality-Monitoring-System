#include <WiFi.h>
#include <HTTPClient.h>
#include <ThingSpeak.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_Sensor.h>
#include <SDS011.h>

const char* SSID = "D-LINK_Guest";
const char* PASSWORD = "vasavicc";

const char* THINGSPEAK_WRITE_API_KEY = "6DPKYXMAYCKYMZSS"; 
const char* THINGSPEAK_READ_API_KEY = "6S5SJ75P959H270U";

WiFiClient client;

Adafruit_AHTX0 aht;
SDS011 sds;

unsigned long previousMillis = 0;
const long interval = 10000;

void setup() {
  Serial.begin(115200);

  // Initialize Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Wi-Fi connected.");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Initialize AHT10 sensor
  if (!aht.begin()) {
    Serial.println("Failed to initialize AHT10 sensor!");
    while (1);
  }

  // Initialize SDS011 sensor
  sds.begin(14, 12); // Use GPIO14 as RX and GPIO12 as TX
  Serial.println("Sensors initialized successfully.");
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to read sensors and send data
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read temperature and humidity from AHT10
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    float temperature = temp.temperature;
    float humidityValue = humidity.relative_humidity;

    // Read PM2.5 and PM10 from SDS011
    float pm25, pm10;
    int error = sds.read(&pm25, &pm10);

    if (error == 0) {
      Serial.println("Sensor readings:");
      Serial.print("Temperature: "); Serial.println(temperature);
      Serial.print("Humidity: "); Serial.println(humidityValue);
      Serial.print("PM2.5: "); Serial.println(pm25);
      Serial.print("PM10: "); Serial.println(pm10);

      // Push data to ThingSpeak
      ThingSpeak.setField(1, temperature);
      ThingSpeak.setField(2, humidityValue);
      ThingSpeak.setField(3, pm25);
      ThingSpeak.setField(4, pm10);

      int responseCode = ThingSpeak.writeFields(0, THINGSPEAK_WRITE_API_KEY);
      if (responseCode == 200) {
        Serial.println("Data sent to ThingSpeak successfully.");
      } else {
        Serial.print("Failed to send data. Error code: "); Serial.println(responseCode);
      }
    } else {
      Serial.println("Failed to read data from SDS011 sensor.");
    }
  }

  // Handle Wi-Fi reconnection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Reconnecting...");
    }
    Serial.println("Wi-Fi reconnected.");
  }
}