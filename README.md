# Smartes Aquarium mit ESP32

## Überblick
Dieses Projekt steuert die Beleuchtung eines Aquariums mithilfe eines ESP32-Mikrocontrollers. Es überwacht, ebenso die Temperatur im Aquarium mithilfe eines DS18B20-Temperatursensors und zeigt diese auf einem OLED-Display (SSD1306) an. Zusätzlich bietet es eine Weboberfläche, über die Benutzer Start- und Endzeiten für das Aquariumlicht einstellen können. Das Licht wird basierend auf der aktuellen Uhrzeit und den gewählten Start- und Endzeiten automatisch ein- oder ausgeschaltet.

## Funktionen
- **WLAN-Verbindung**: Der ESP32 verbindet sich mit einem WLAN-Netzwerk.
- **Webserver**: Eine Web-Oberfläche zur Steuerung der LED-Beleuchtung.
- **Zeitschaltung**: Automatisches Ein- und Ausschalten der LEDs zu konfigurierbaren Zeiten.
- **Manueller Modus**: LEDs können über Buttons auf der Web-Oberfläche an- und ausgeschaltet werden. (Noch nicht funktional)
- **Temperaturmessung**: Ein DS18B20-Temperatursensor misst die Wassertemperatur.
- **NTP-Zeitabfrage**: Der ESP32 synchronisiert sich mit einem NTP-Server, um die exakte Uhrzeit zu erhalten.

## Funktionsweise
- Temperaturmessung: Der DS18B20-Sensor misst die Temperatur, die stündlich erfasst und gespeichert wird.
- Lichtsteuerung: Der Benutzer kann die Start- und Endzeiten für das Aquariumlicht einstellen. Basierend auf der aktuellen Uhrzeit wird das Licht automatisch ein- oder ausgeschaltet.
- Webinterface: Das Webinterface bietet ein Dropdown-Menü zur Auswahl der Start- und Endzeiten für das Licht sowie eine grafische Darstellung der Temperatur über 24 Stunden.
- OLED-Display: Das Display zeigt die aktuelle Temperatur sowie die Start- und Endzeiten des Lichts an.

## Benötigte Hardware
- ESP32
- Buck Converter
- 12V LED
- 12V Netzteil (abhängig von den LEDs)
- DS18B20-Temperatursensor
- IRF3708
- SSD1306 OLED-Display

## Software & Bibliotheken
Benötigte Arduino-Bibliotheken:
- `WiFi.h`
- `WebServer.h`
- `DallasTemperature.h`
- `OneWire.h`
- `Wire.h`
- `time.h`
- `Adafruit_GFX.h`
- `Adafruit_SSD1306.h`


## Einrichtung & Installation
1. **Arduino IDE vorbereiten**:
   - Installiere die ESP32-Bibliotheken.
   - Füge die benötigten Bibliotheken hinzu.
2. **Code hochladen**:
   - Passe die WLAN-Zugangsdaten an.
   - Lade den Sketch auf den ESP32 hoch.
3. **Hardware anschließen**:
   - LEDs und Sensoren mit dem ESP32 verbinden.
4. **Webinterface aufrufen**:
   - Die IP-Adresse des ESP32 im Browser öffnen, um die Steuerung zu nutzen.
