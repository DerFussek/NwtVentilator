#include "Arduino.h"
#include "A4988.h"
#include "LowerStepper.h"
#define MOTOR_STEPS 200
#define MICROSTEPS 16
#define RPM 20

#define DIR 8
#define STEP 7
#define SLEEP 6
#define MS1 5
#define MS2 4
#define MS3 3

#define STOPP 2  //Pin 21 hat die höchste Interrupt-Priorität auf dem Arduino-Mega

LowerStepper stepper(RPM, DIR, STEP, SLEEP, MS1, MS2, MS3, STOPP);

#include "UpperStepper.h"

#define upper_DIR 27
#define upper_STEP 26
#define upper_SLEEP 25
#define upper_MS3 24
#define upper_MS2 23
#define upper_MS1 22

UpperStepper stepper2(RPM, upper_DIR, upper_STEP, upper_SLEEP, upper_MS1, upper_MS2, upper_MS3);
int position = 0;
#include "GSM.h"
//GSM gsm(12, 11, 10, 9, 8);

//=========US-Sensoren=========//
struct Sensor {
  uint8_t trig;
  uint8_t echo;
};

Sensor sensoren[6] = {{43, 42}, {45, 44}, {47, 46}, {48, 49}, {50, 51}, {41,40}};
long messwerte[12];
long _messwerte[12];
int winkel = 0;
const int MAX_DISTANZ = 80; //[in cm]
int grad[12] = {0, 60, 120, 180, 240, 300, 30, 90, 150, 210, 270, 330};

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial.println("Hallo");
  pinMode(STOPP, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(STOPP), LowerStepper::stopInterrupt, FALLING);

  
    for (int i = 0; i < 6; i++) {
      pinMode(sensoren[i].trig, OUTPUT);
      pinMode(sensoren[i].echo, INPUT);
    }
  
  stepper2.begin();
  stepper2.test();
  stepper.begin();
}

#include "Messager.h"
Modus m = NONE;
Modus lastModus;
Modus tempModus;
bool on = false;
bool off = true;
bool manuel;

uint8_t stufe = 0;

void loop() {
  Messager.receiver.read(tempModus, stufe);
  if (tempModus != lastModus && tempModus != -1) {
    lastModus = tempModus;
    m = lastModus;
    Serial.print("Modus: ");
    Serial.println(m);
  }

  if (m == OFF) {
    on = false;
    manuel = false;
    off = true;
    stufe = 0;
  } else if (m == ON) {
    off = false;
    manuel = false;
    on = true;
    stufe = 1;
  } else if (m == MANUAL) {
    on = false;
    off = false;
    manuel = true;
    stufe = 1;
  }

  delay(250);

  if (true) {
    const int SENSOREN = 6;
    long messrunde1[SENSOREN];
    long messrunde2[SENSOREN];
    int grad[6] = {0, 60, 120, 180, 240, 300};

    // Messung in Startposition (0°)
    for (int i = 0; i < SENSOREN; i++) {
      messrunde1[i] = messung(sensoren[i].trig, sensoren[i].echo);
      delay(50);
    }

    // Drehen auf +120°
    //stepper.move(120);
    delay(100);

    // Zweite Messung
    for (int i = 0; i < SENSOREN; i++) {
      messrunde2[i] = messung(sensoren[i].trig, sensoren[i].echo);
      delay(50);
    }

    // Zurückdrehen auf Ausgangsposition
    //stepper.move(-120);
    delay(100);

    // Veränderung analysieren
    int maxDiff = 5;
    int zielSensor = -1;

    for (int i = 0; i < SENSOREN; i++) {
      int diff = abs(messrunde1[i] - messrunde2[i]);
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.print(" (");
      Serial.print(grad[i]);
      Serial.print("°): ");
      Serial.print(messrunde1[i]);
      Serial.print(" cm -> ");
      Serial.print(messrunde2[i]);
      Serial.print(" cm => Diff: ");
      Serial.println(diff);

      if (diff > maxDiff && messrunde2[i] > 0 && messrunde1[i] > 0) {
        maxDiff = diff;
        zielSensor = i;
      }
    }

    if (zielSensor != -1) {
      Serial.print("Bewegung erkannt bei Sensor ");
      Serial.print(zielSensor);
      Serial.print(" — Richtung: ");
      Serial.print(grad[zielSensor]);
      Serial.println("°");
      movestepper(grad[zielSensor]);
    } else {
      Serial.println("Keine signifikante Bewegung erkannt.");
    }

    Serial.print("Motor = ");
    Serial.println(position);

  }
}


void movestepper(int ziel) {
  int delta = ziel - position;

  // Kürzesten Weg wählen (optional, falls z. B. -270° besser als +90° wäre)
  if (delta > 180) delta -= 360;
  if (delta < -180) delta += 360;

  // Schrittmotor ansteuern
  stepper2.getStepper().enable();
  stepper2.getStepper().rotate(delta * -1);  // oder z. B. moveDegrees(delta); falls du so eine Methode hast

  // Position aktualisieren (Modulo 360, falls du willst)
  position = (position + delta) % 360;
  if (position < 0) position += 360;  // Immer positiv halten
}

long messung(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long dauer = pulseIn(echoPin, HIGH, 30000);
  long entfernung = dauer * 0.034 / 2;
  return entfernung;
}

long gefilterteMessung(uint8_t trig, uint8_t echo) {
  long d = messung(trig, echo);
  return d;
}


