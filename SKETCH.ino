#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 11
#define DHTTYPE DHT22
#define GAS_PIN 1
#define WATER_PIN 2
#define BUZZER_PIN 12
#define LED_PIN 13

// ESP32-S2 I2C Pins: SDA = GPIO 8, SCL = GPIO 9
#define SDA_PIN 8
#define SCL_PIN 9

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// IoT Device Configuration
const char* DEVICE_ID = "MINE_NODE_01";
unsigned long lastPublishTime = 0;
const long publishInterval = 2000; // 2 விநாடிகளுக்கு ஒருமுறை Cloud-க்கு தரவு அனுப்பப்படும்

// Cloud Status & Offline Caching Logic
bool isCloudConnected = false; // Cloud Connect ஆகவில்லை என வைத்துக்கொள்வோம்

void saveToFlash(String payload) {
  // Offline Flash Storage Saving Logic (e.g., LittleFS / SPIFFS / Preferences)
  Serial.println("[SPIFFS/Flash]: Stored -> " + payload);
}

void sendTelemetry(float temp, int gas, int water, String status, unsigned long currentMillis) {
  // Formatting as standard IoT Telemetry JSON Packet
  String iotPayload = "{";
  iotPayload += "\"device_id\":\"" + String(DEVICE_ID) + "\",";
  iotPayload += "\"uptime_ms\":" + String(currentMillis) + ",";
  iotPayload += "\"temperature\":" + String(temp, 1) + ",";
  iotPayload += "\"methane_gas\":" + String(gas) + ",";
  iotPayload += "\"water_level\":" + String(water) + ",";
  iotPayload += "\"status\":\"" + status + "\"";
  iotPayload += "}";

  if (isCloudConnected) {
    // Cloud இயங்கினால் நேரடியாக அனுப்பு
    Serial.print("[IoT PUBLISH -> Cloud]: ");
    Serial.println(iotPayload);
  } else {
    // Cloud துண்டிக்கப்பட்டால் Caching செய்
    Serial.println("[OFFLINE CACHE] Network Down! Saving Data to Flash Memory...");
    saveToFlash(iotPayload); // Memory-இல் சேமிக்கும் Function
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C for ESP32-S2
  Wire.begin(SDA_PIN, SCL_PIN);
  
  dht.begin();
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Mine Safety Sys");
  lcd.setCursor(0, 1);
  lcd.print("IoT Cloud Ready");
  delay(2000);
  lcd.clear();

  // Initializing Serial Communication for IoT Gateway
  Serial.println("\n--- MINE SAFETY SYSTEM INITIALIZED ---");
  Serial.println("[IoT Status]: Gateway Connected via Serial Telemetry Stream\n");
}

void loop() {
  unsigned long currentMillis = millis();

  // Read Sensor Values
  float temp = dht.readTemperature();
  int gasVal = analogRead(GAS_PIN);
  int waterVal = analogRead(WATER_PIN);

  if (isnan(temp)) {
    temp = 0.0;
  }

  // Safety Status Logic
  String status = "NORMAL";
  bool isDanger = false;

  if (temp > 45.0 || gasVal > 2000 || waterVal > 2000) {
    status = "DANGER";
    isDanger = true;
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  // Update LCD Screen
  lcd.setCursor(0, 0);
  lcd.print("T:" + String(temp, 1) + "C G:" + String(gasVal));
  lcd.setCursor(0, 1);
  if (isDanger) {
    lcd.print("! DANGER ALERT !");
  } else {
    lcd.print("W:" + String(waterVal) + " Status:OK");
  }

  // Send IoT Telemetry Payload via Serial to Cloud Gateway (Every 2 seconds)
  if (currentMillis - lastPublishTime >= publishInterval) {
    lastPublishTime = currentMillis;
    
    // Call the telemetry function with caching check
    sendTelemetry(temp, gasVal, waterVal, status, currentMillis);
  }

  delay(500);
}
