# AirQualityEsp8266

This project uses an ESP8266 microcontroller to monitor air quality (PM1.0, PM2.5, PM10), temperature, and humidity using a PMS5003 air quality sensor, a DHT22 sensor, and displays the data on an SSD1306 OLED display.

## Hardware Requirements
- **ESP8266** (e.g., NodeMCU, Wemos D1 Mini)
- **PMS5003** Air Quality Sensor
- **DHT22** Temperature and Humidity Sensor
- **SSD1306 OLED Display** (128x64, I2C interface)
- Jumper wires and a breadboard or custom PCB
- USB cable for programming and powering the ESP8266

## Pin Connections
| ESP8266 Pin | Sensor/Display Pin | Description |
|-------------|--------------------|-------------|
| D1          | SCL (SSD1306)      | I2C clock for OLED display |
| D2          | SDA (SSD1306)      | I2C data for OLED display  |
| D4          | DATA (DHT22)       | Data pin for DHT22 sensor  |
| TX (GPIO 1) | RX (PMS5003)       | Serial receive for PMS5003 |
| RX (GPIO 3) | TX (PMS5003)       | Serial transmit for PMS5003 |

### Note on TX/RX Connections
The connection of **ESP8266 TX to PMS5003 RX** and **ESP8266 RX to PMS5003 TX** may seem confusing because, typically, TX connects to RX and vice versa. This is correct and intentional:
- **ESP8266 TX (transmit)** sends data to **PMS5003 RX (receive)**.
- **ESP8266 RX (receive)** accepts data from **PMS5003 TX (transmit)**.
This follows standard UART communication protocol where the TX pin of one device connects to the RX pin of the other. If you are experiencing issues, ensure:
- The PMS5003 is powered correctly (5V VCC, GND).
- The serial baud rate matches (9600 for PMS5003).
- No loose connections or incorrect wiring.

## Software Requirements
- **Arduino IDE** or compatible IDE (e.g., PlatformIO)
- **Libraries**:
  - `Adafruit_GFX` (for OLED display)
  - `Adafruit_SSD1306` (for OLED display)
  - `DHT` (for DHT22 sensor)
- Install the ESP8266 board package in Arduino IDE (URL: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`)

## Installation
1. **Set up Arduino IDE**:
   - Install the required libraries via the Library Manager.
   - Add the ESP8266 board package in the Boards Manager.
2. **Connect the hardware** as per the pin connections table above.
3. **Upload the code**:
   - Open the `AirQualityEsp8266.ino` file in the Arduino IDE.
   - Select your ESP8266 board (e.g., NodeMCU 1.0) and the correct COM port.
   - Upload the code to the ESP8266.
4. **Monitor output**:
   - Open the Serial Monitor (9600 baud) to view PM1.0, PM2.5, PM10, temperature, and humidity readings.
   - The OLED display will show the same data, updated every second.

## Code Overview
The code (`AirQualityEsp8266.ino`) performs the following:
- Reads PM1.0, PM2.5, and PM10 values from the PMS5003 sensor via UART.
- Reads temperature and humidity from the DHT22 sensor.
- Displays all data on the SSD1306 OLED display and Serial Monitor.
- Includes error handling for:
  - OLED initialization failure.
  - Incomplete or invalid PMS5003 data.
  - Failed DHT22 readings.

## Usage
- Power the ESP8266 via USB or an external 5V source.
- The OLED will display:
  - PM1.0, PM2.5, and PM10 concentrations (in µg/m³).
  - Temperature (in °C) and humidity (in %).
- Serial Monitor output mirrors the OLED display for debugging.
- Data updates every 1 second.

## Troubleshooting
- **No data on OLED**: Check D1/D2 connections and ensure the I2C address is `0x3C` (modify in code if different).
- **No PMS5003 data**: Verify TX/RX connections, ensure 9600 baud rate, and check PMS5003 power (5V).
- **DHT22 errors**: Ensure D4 is connected correctly and the sensor is powered (3.3V or 5V, depending on model).
- **Serial Monitor issues**: Ensure the correct baud rate (9600) is selected.

## Notes
- The PMS5003 requires a 5V power supply, while the ESP8266 operates at 3.3V. Ensure the PMS5003 VCC is connected to 5V (e.g., VIN on NodeMCU) and GND is shared.
- The DHT22 may require a 4.7k–10k pull-up resistor between DATA and VCC.
- The code assumes no temperature calibration (e.g., no -5°C offset). If calibration is needed, modify the `loop()` function with a comment explaining the adjustment.

## License
This project is open-source and available under the MIT License.
