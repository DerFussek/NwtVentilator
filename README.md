# Aufbau
- **Tests** – unabhängige Programme zum Prüfen einzelner Komponenten
- **OOP_NwtVentilator** – objektorientierte Version mit zwei Arduino-Sketchen
- **NwtVentilator** – ältere nicht objektorientierte Variante

## Hardwareaufbau
Der Ventilator besteht aus zwei miteinander verbundenen Arduino Mega 2560.

**Arduino 1 (Bedienung)**
- Infrarot-Empfänger am Pin 13 für die Fernbedienung
- LED-Strip aus acht NeoPixel-LEDs am Pin 12
- Drei Status-LEDs (Ampel) an den Pins 45 (Rot), 41 (Gelb) und 43 (Grün)
- Kommunikation mit dem zweiten Arduino über `Serial2`

**Arduino 2 (Antrieb)**
- Zwei Nema17 Schrittmotoren an A4988 Treibern
  - unterer Motor: Pins 8–5 sowie 2 für den Stopp-Schalter
  - oberer Motor: Pins 27–22
- DC-Lüftermotor an den Pins 10–13 und PWM an Pin 9
- Sechs Ultraschallsensoren (Trig/Echo) an den Pins
  `(43/42), (45/44), (47/46), (48/49), (50/51), (41/40)`
- Verbindung zu Arduino 1 über `Serial2`

## Funktion der Sketche
**Arduino1/Arduino1.ino** verarbeitet die Signale der Fernbedienung und stellt den
aktuellen Modus über das Ampel-Display und den LED-Ring dar. Die ausgewählte
Stufe sowie die Position werden per serieller Schnittstelle an den zweiten Arduino
übermittelt.

**Arduino2/Arduino2.ino** empfängt diese Daten und steuert damit die Schrittmotoren
und den Lüfter. Die Ultraschallsensoren werden ausgewertet, um die Position des
Lüfters selbsttätig zu korrigieren.

## Benötigte Bibliotheken
Die Sketche setzen auf einige externe Arduino-Bibliotheken:
- [IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote) für die IR-Fernbedienung
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) für den LED-Ring
- [StepperDriver](https://github.com/laurb9/StepperDriver) (A4988) für die
  Ansteuerung der Nema17-Motoren

Alle Bibliotheken lassen sich über den Arduino-Bibliotheksmanager installieren.

## Kompilieren und Aufspielen
Am einfachsten wird das Projekt mit der
[Arduino-CLI](https://arduino.github.io/arduino-cli/) gebaut. Nach dem Installieren
müssen die benötigten Bibliotheken eingebunden und als Board der
`arduino:avr:mega` (Arduino Mega 2560) ausgewählt werden:

```bash
# Beispiel für Arduino 1
arduino-cli compile --fqbn arduino:avr:mega OOP_NwtVentilator/Arduino1
arduino-cli upload  --fqbn arduino:avr:mega -p /dev/ttyACM0 OOP_NwtVentilator/Arduino1

# Beispiel für Arduino 2
arduino-cli compile --fqbn arduino:avr:mega OOP_NwtVentilator/Arduino2
arduino-cli upload  --fqbn arduino:avr:mega -p /dev/ttyACM1 OOP_NwtVentilator/Arduino2
```

Die Ports `/dev/ttyACM0` bzw. `/dev/ttyACM1` sind je nach System anzupassen. Die
ältere `NwtVentilator`-Variante lässt sich auf die gleiche Weise kompilieren.
