# Smart Aquarium with ESP32

## Overview
This project manages the lighting of an aquarium using an ESP32 microcontroller. It also monitors the temperature in the aquarium using a DS18B20 temperature sensor and displays it on an OLED display (SSD1306). Additionally, it offers a web interface that allows users to set start and end times for the aquarium light. The light is automatically turned on or off based on the current time and the configured schedule.

## Features
- **Wi-Fi Connection**: The ESP32 connects to a Wi-Fi network.
- **Web Server**: A web interface to control the LED lighting.
- **Timer Functionality**: Automatically turns the LEDs on and off at configurable times.
- **Temperature Monitoring**: A DS18B20 temperature sensor measures the water temperature.
- **NTP Time Synchronization**: The ESP32 syncs with an NTP server to obtain the accurate current time.

## How It Works
- **Temperature Monitoring**: The DS18B20 sensor measures the temperature, which is recorded and stored hourly.
- **Lighting Control**: Users can set the start and end times for the aquarium light. Based on the current time, the light turns on or off automatically.
- **Web Interface**: The interface provides dropdown menus to select the lighting start and end times, along with a graphical display of the temperature over 24 hours.
- **OLED Display**: The screen shows the current temperature and the lighting schedule (start and end times).

## Required Hardware
- ESP32
- Buck Converter 12V to 5V 
- 12V LED
- 12V Power Supply (depending on the LEDs)
- DS18B20 Temperature Sensor
- IRF3708
- SSD1306 OLED Display
- 4.7kΩ pull-up resistor
- 10kΩ pull-up resistor

## Software & Libraries
Required Arduino libraries:
- `WiFi.h`
- `WebServer.h`
- `DallasTemperature.h`
- `OneWire.h`
- `Wire.h`
- `time.h`
- `Adafruit_GFX.h`
- `Adafruit_SSD1306.h`

## Setup & Installation
1. **Prepare the Arduino IDE**:
   - Install the ESP32 board libraries.
   - Add the required libraries.
2. **Upload the Code**:
   - Update your Wi-Fi credentials.
   - Upload the sketch to the ESP32.
3. **Connect the Hardware**:
   - Connect the LEDs and sensors to the ESP32.
   - DS18B20 requires a 4.7kΩ pull-up resistor between DATA and VCC
   - 10kΩ resistor between GPIO16 and the gate of the IRF3708 MOSFET
4. **Open the Web Interface**:
   - Open the IP address of the ESP32 in your browser to access the control panel.

## Project photos  
**Front and rear views of the Smart Aquarium project**

![SmartAquarium](https://github.com/user-attachments/assets/ec97f29c-efd0-41ce-901f-fd5413b42e33)

![SmartAquarium_rückseite](https://github.com/user-attachments/assets/1c4a280e-f600-414a-b95d-9bd2e2317a92)

## Circuit Diagram
![image](https://github.com/user-attachments/assets/a1b1169d-eb25-4af0-9cf1-22fc8773d1b1)

