#include "Arduino.h"
// Pin-Definition
const int trigPin = 9;
const int echoPin = 10;

// Variable zur Entfernung
long duration;
float distance;

void setup() {
  // Serielle Verbindung starten
  Serial.begin(115200);

  // Pin-Modus festlegen
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Sicherstellen, dass Trigger LOW ist
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // 10µs HIGH-Puls senden
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Dauer des Echos messen
  duration = pulseIn(echoPin, HIGH);

  // Entfernung berechnen
  distance = (duration * 0.0343) / 2;

  // Entfernung auf Serial Monitor ausgeben

  Serial.print(distance);
  Serial.println("cm");


  // kleine Pause
  delay(250);
}
