#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define SOIL_PIN 3
#define RELAY_PIN 8

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const unsigned long pumpRunTime = 3000;
const unsigned long checkDelay = 5000;

// Prototype target band inferred from E. blascoi's cool, moist Kodaikanal shola habitat.
// Tune these values after you collect real plant response data.
const float idealTempMin = 19.0;
const float idealTempMax = 23.0;
const float safeTempMin = 16.0;
const float safeTempMax = 28.0;
const float idealHumidityMin = 70.0;
const float idealHumidityMax = 85.0;
const float safeHumidityMin = 60.0;
const float safeHumidityMax = 90.0;

String environmentStatus(float temperature, float humidity, bool soilDry) {
  if (soilDry || temperature < safeTempMin || temperature > safeTempMax || humidity < safeHumidityMin || humidity > safeHumidityMax) {
    return "CRITICAL";
  }

  if (temperature >= idealTempMin && temperature <= idealTempMax && humidity >= idealHumidityMin && humidity <= idealHumidityMax) {
    return "IDEAL";
  }

  return "WATCH";
}

String actionTip(float temperature, float humidity, bool soilDry) {
  if (soilDry) return "WATER_SOIL";
  if (temperature > idealTempMax) return "MOVE_SHADE";
  if (temperature < idealTempMin) return "KEEP_WARM";
  if (humidity < idealHumidityMin) return "RAISE_HUMID";
  if (humidity > idealHumidityMax) return "AIR_FLOW";
  return "MAINTAIN";
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  lcd.init();
  lcd.backlight();

  pinMode(SOIL_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH);

  lcd.setCursor(0, 0);
  lcd.print("Plant Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("ERROR,DHT11_READ_FAILED");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DHT11 Error");
    lcd.setCursor(0, 1);
    lcd.print("Check wiring");

    delay(checkDelay);
    return;
  }

  int soilDigital = digitalRead(SOIL_PIN);
  bool soilDry = (soilDigital == HIGH);
  String envStatus = environmentStatus(temperature, humidity, soilDry);
  String tip = actionTip(temperature, humidity, soilDry);

  int pumpStatus = 0;
  int pumpDuration = 0;
  int wateringEvent = 0;

  if (soilDry) {
    pumpStatus = 1;
    pumpDuration = 3;
    wateringEvent = 1;

    digitalWrite(RELAY_PIN, LOW);
    delay(pumpRunTime);
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, HIGH);
  }

  Serial.print(temperature);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.print(soilDigital);
  Serial.print(",");
  Serial.print(pumpStatus);
  Serial.print(",");
  Serial.print(pumpDuration);
  Serial.print(",");
  Serial.print(wateringEvent);
  Serial.print(",");
  Serial.print(envStatus);
  Serial.print(",");
  Serial.println(tip);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 0);
  lcd.print("C H:");
  lcd.print(humidity, 0);
  lcd.print("%");

  lcd.setCursor(0, 1);
  if (envStatus == "IDEAL") {
    lcd.print("Ideal ");
  } else if (envStatus == "WATCH") {
    lcd.print("Watch ");
  } else {
    lcd.print("Alert ");
  }
  lcd.print(soilDry ? "Dry " : "Wet ");
  lcd.print(pumpStatus ? "P:ON" : "P:OFF");

  delay(checkDelay);
}
