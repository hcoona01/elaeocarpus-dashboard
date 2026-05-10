# Elaeocarpus Blascoi Smart Survival Project

## Files In This Folder

- `elaeocarpus_blascoi_synthetic_sensor_dataset_50000.csv`: synthetic 50,000-row training dataset.
- `arduino_dht11_lcd_pump.ino`: Arduino code for DHT11, LM393 soil sensor, relay, pump, and LCD.
- `colab_train_model.py`: copy this into Google Colab to train and save the ML model.
- `index.html`: working local dashboard preview.
- `server.py`: simple local web server fallback.

## Hardware Connections

DHT11:

```text
VCC  -> Arduino 5V
DATA -> Arduino D2
GND  -> Arduino GND
```

LM393 soil moisture:

```text
VCC -> Arduino 5V
GND -> Arduino GND
DO  -> Arduino D3
```

Relay:

```text
VCC -> Arduino 5V
GND -> Arduino GND
IN  -> Arduino D8
```

LCD I2C:

```text
GND -> Arduino GND
VCC -> Arduino 5V
SDA -> Arduino A4
SCL -> Arduino A5
```

Pump with external battery:

```text
Battery + -> Relay COM
Relay NO  -> Pump +
Pump -    -> Battery -
```

Do not power the pump from the Arduino.

## Dashboard

The dashboard is currently served at:

```text
http://127.0.0.1:8501/
```

It works without Python packages. Use Manual Reading first to test it.

Arduino connection from dashboard:

```text
1. Upload `arduino_dht11_lcd_pump.ino` to Arduino.
2. Close Arduino IDE Serial Monitor.
3. Open dashboard in Chrome or Edge at http://127.0.0.1:8501/.
4. Click Detect Ports.
5. If no port appears, click Connect Arduino and choose the Arduino COM port.
6. Keep baud rate at 9600.
7. Click Read One Line for one reading.
8. Click Start Live Read for continuous dashboard records.
9. Click Disconnect before unplugging Arduino.
```

Important:

```text
Only one app can use the Arduino serial port at a time.
If Arduino IDE Serial Monitor is open, the dashboard cannot connect.
Web Serial works best in Chrome or Edge.
```

## Research-Based Target Settings

The prototype target band is:

```text
Ideal temperature: 19-23 C
Safe temperature: 16-28 C
Ideal humidity: 70-85%
Safe humidity: 60-90%
Soil: wet/moist, not dry
```

Why these settings are used:

```text
E. blascoi is reported from Vattakanal/Bear Shola in the Palani Hills around 2011-2200 m elevation.
That habitat is a cool, moist shola forest environment near Kodaikanal.
Kodaikanal climate references show average temperatures around 18-23 C and humid monsoon conditions.
```

Use this wording in the report:

```text
The target temperature and humidity bands are prototype settings inferred from published habitat records and Kodaikanal climate data. They are not guaranteed biological optima and should be refined using real sensor and plant-response data.
```

The Arduino serial format expected by the dashboard is:

```text
temperature,humidity,soilDigital,pumpStatus,pumpDuration,wateringEvent,environmentStatus,actionTip
```

Example:

```text
27.00,65.00,1,1,3,1,CRITICAL,WATER_SOIL
```

Environment status values:

```text
IDEAL    = temperature and humidity are inside the ideal shola-habitat band, and soil is not dry
WATCH    = readings are outside ideal but still inside the wider safe band
CRITICAL = soil is dry, temperature is unsafe, or humidity is unsafe
```

## ML Training

In Google Colab:

1. Upload the CSV dataset.
2. Copy and run `colab_train_model.py`.
3. Download:

```text
elaeocarpus_survival_model.pkl
label_encoder.pkl
```

The current local preview uses a simple built-in prediction rule so it works without installing packages. The Colab model is for your report and later Python dashboard integration.
