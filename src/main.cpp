#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "time.h"

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// NTP-Server for current time
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0; // 0 Wintertime; 3600 Summertime

WebServer server(8080);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define ONE_WIRE_BUS 4  // GPIO for DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define MOSFET_PIN 16

int startHour = 10; // Standard start time
int endHour = 22;   // Standard end time

// Graph for temperature
float hourlyTemperatures[24] = {0}; // Array for 24 hours

// HTML with Dropdown menu
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="de">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    body { 
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
      background-color: #f0f8ff; 
      margin: 0;
      padding: 0;
      color: #333;
      text-align: center;
    }

    h1 {
      color: #1c509d;
      margin-top: 30px;
      font-size: 36px;
    }

    p {
      font-size: 16px;
      margin: 10px 0;
    }

    .container {
      margin: 20px auto;
      width: 90%;
      max-width: 400px;
    }

    select {
      padding: 10px;
      font-size: 16px;
      border: 1px solid #ccc;
      border-radius: 10px;
      margin: 10px;
      width: 25%;
      background-color: #f9f9f9;
      box-sizing: border-box;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
      text-align: center;
    }

    select:focus {
      border-color: #1c509d;
      outline: none;
    }

    .btn {
      padding: 12px 20px;
      font-size: 16px;
      background-color: #1c509d;
      color: white;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
      transition: all 0.3s ease;
      margin-top: 10px;
    }

    .btn:hover {
      background-color: #64a2ff;
      transform: translateY(-2px);
    }

    .btn:active {
      transform: translateY(2px);
    }

    .temp-info {
      margin-top: 30px;
      margin-bottom: 40px;
      font-size: 18px;
      font-weight: bold;
      color: #1c509d;
    }

    .chart-container {
      margin-top: 40px;
      max-width: 800px;
      margin-left: auto;
      margin-right: auto;
    }

    .footer {
      font-size: 12px;
      color: #888;
      margin-top: 30px;
    }
  </style>
</head>
<body>

  <h1>Smartes Aquarium</h1>

  <div class="container">
    <p class="temp-info" id="currentTemp">Aktuelle Temperatur: Wird geladen...</p>

    <p>Startzeit wählen:</p>
    <select id="startHour">%STARTHOUR_OPTIONS%</select>

    <p>Endzeit wählen:</p>
    <select id="endHour">%ENDHOUR_OPTIONS%</select>

  </div>
    <button class="btn" onclick="setTimes()">Speichern</button>

  <div class="chart-container">
    <canvas id="tempChart"></canvas>
  </div>

  <script>
    function setTimes() {
      var start = document.getElementById("startHour").value;
      var end = document.getElementById("endHour").value;
      fetch("/setTimes?start=" + start + "&end=" + end);
      alert("Zeiten gespeichert!");
    }

    async function loadTemperatureData() {
      let response = await fetch("/getTemperatureData");
      let data = await response.json();

      let tempData = data.temperatures;
      let currentTemp = data.currentTemp;

      // update graph
      const ctx = document.getElementById('tempChart').getContext('2d');
      new Chart(ctx, {
        type: 'line',
        data: {
          labels: Array.from({ length: 24 }, (_, i) => i + ":00"),
          datasets: [{
            label: 'Temperatur (°C)',
            data: tempData,
            borderColor: '#1c509d',
            backgroundColor: 'rgba(28, 80, 157, 0.2)',
            fill: true,
            lineTension: 0.2,
            pointRadius: 0,
          }]
        },
        options: {
          responsive: true,
          scales: {
            y: {
              min: 17,
              ticks: {
                stepSize: 0.2,
                font: { size: 12 }
              }
            }
          }
        }
      });

      // Display current temperature
      document.getElementById("currentTemp").innerHTML = `Aktuelle Temperatur: ${currentTemp.toFixed(2)} °C`;
    }

    setInterval(loadTemperatureData, 10000);
  </script>

</body>
</html>)rawliteral";

// Dropdown for hour selection
String generateHourOptions(int selectedHour) {
    String options = "";
    for (int i = 0; i < 24; i++) {
        options += "<option value='" + String(i) + "'";
        if (i == selectedHour) options += " selected";
        options += ">" + String(i) + ":00</option>";
    }
    return options;
}

// Deliver HTML page with current times
void handleRoot() {
    String html = index_html;
    html.replace("%STARTHOUR_OPTIONS%", generateHourOptions(startHour));
    html.replace("%ENDHOUR_OPTIONS%", generateHourOptions(endHour));

    server.sendHeader("Content-Type", "text/html; charset=utf-8");
    server.send(200, "text/html", html);
}

// API to store the times
void handleSetTimes() {
    if (server.hasArg("start") && server.hasArg("end")) {
        startHour = server.arg("start").toInt();
        endHour = server.arg("end").toInt();
        Serial.printf("Neue Zeiten: Start=%d Uhr, Ende=%d Uhr\n", startHour, endHour);
        server.send(200, "text/plain", "Zeiten gespeichert");
    } else {
        server.send(400, "text/plain", "Fehlende Parameter");
    }
}

// API for temperature data
void handleGetTemperatureData() {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        int currentHour = timeinfo.tm_hour;
        float currentTemp = hourlyTemperatures[currentHour];

        // JSON string with all temperatures + current temperature
        String json = "{";
        json += "\"temperatures\":[";
        for (int i = 0; i < 24; i++) {
            json += String(hourlyTemperatures[i]);
            if (i < 23) json += ",";
        }
        json += "],";
        json += "\"currentTemp\":" + String(currentTemp);
        json += "}";

        server.send(200, "application/json", json);
    } else {
        server.send(500, "application/json", "{\"error\":\"Zeit konnte nicht abgerufen werden\"}");
    }
}

// LED control based on the current time
void checkTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Fehler beim Abrufen der Zeit");
        return;
    }

    int hour = timeinfo.tm_hour;
    int min = timeinfo.tm_min;

    Serial.printf("Aktuelle Uhrzeit: %d:%d Uhr\n", hour, min);

    // If current time is between start and end time -> turn LED on
    if ((startHour <= endHour && hour >= startHour && hour < endHour) || 
    (startHour > endHour && (hour >= startHour || hour < endHour))) {
        digitalWrite(MOSFET_PIN, HIGH); // MOSFET on -> LED on
        Serial.println("LED AN");
    } else {
        digitalWrite(MOSFET_PIN, LOW);  // MOSFET off -> LED off
        Serial.println("LED AUS");
    }
}

void displayTemperature(float temp) {
    display.clearDisplay(); 

    display.setTextSize(2);
    display.setCursor(0, 10);
    display.print(temp);
    display.print(" C");
        
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.print("Licht an von:");
    
    display.setCursor(0, 50);
    display.print(startHour);
    display.print(" Uhr - ");
    
    display.print(endHour);
    display.print(" Uhr");
    display.display(); 
}


void updateTemperature() {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        int currentHour = timeinfo.tm_hour;
        sensors.requestTemperatures();
        float temp = sensors.getTempCByIndex(0);
        hourlyTemperatures[currentHour] = temp;
        Serial.printf("Temperatur um %d:00 Uhr: %.2f °C\n", currentHour, temp);

        displayTemperature(temp);
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("🔄 Verbindung zum WLAN...");
    }

    Serial.println("✅ WLAN verbunden!");
    Serial.println(WiFi.localIP());

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 
    pinMode(MOSFET_PIN, OUTPUT);
    digitalWrite(MOSFET_PIN, LOW); // Default off
    
    sensors.begin();

    server.on("/", handleRoot);
    server.on("/setTimes", handleSetTimes);

    server.on("/getTemperatureData", handleGetTemperatureData);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }

    delay(2000);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.display();


    server.begin();
}

void loop() {
    server.handleClient();
    checkTime(); // Checks the time and turns the LED on/off accordingly
    delay(10000); // Check every n milliseconds    

    updateTemperature();    
}
