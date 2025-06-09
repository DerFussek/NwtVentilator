/*
 * Programm für den zweiten Arduino.
 * Steuert die Schrittmotoren und den Gebläsemotor
 * und verarbeitet die Daten der Ultraschallsensoren.
 */
#include "Arduino.h"                      // Grundlegende Arduino-API
#include "A4988.h"                        // Treiberbibliothek
#include "LowerStepper.h"                 // eigener Schrittmotor unten
#define MOTOR_STEPS 200                    // Schritte pro Umdrehung
#define MICROSTEPS 16                      // Microstep-Auflösung
#define RPM 20                             // Grundgeschwindigkeit

// Pins für den unteren Schrittmotor
#define DIR 8     // Richtung
#define STEP 7    // Schrittimpuls
#define SLEEP 6   // Energiesparen
#define MS1 5     // Microstep-Pin 1
#define MS2 4     // Microstep-Pin 2
#define MS3 3     // Microstep-Pin 3

#define STOPP 2  // Endschalter (Interrupt-fähig)

LowerStepper stepper(RPM, DIR, STEP, SLEEP, MS1, MS2, MS3, STOPP); // Objekt erzeugen

#include "UpperStepper.h"                  // oberer Schrittmotor

// Pins für den oberen Schrittmotor
#define upper_DIR 27   // Richtung
#define upper_STEP 26  // Schrittimpuls
#define upper_SLEEP 25 // Sleep-Pin
#define upper_MS3 24   // Microstep-Pin 3
#define upper_MS2 23   // Microstep-Pin 2
#define upper_MS1 22   // Microstep-Pin 1

UpperStepper stepper2(RPM, upper_DIR, upper_STEP, upper_SLEEP, upper_MS1, upper_MS2, upper_MS3);


#//define DEV

#include "DcMotor.h"                       // Gleichstrommotor für Gebläse
// Reihenfolge: in1, in2, in3, in4, PWM-Pin
DcMotor gsm(10, 11, 12, 13, 9);            // Motorobjekt anlegen

//=========US-Sensoren=========//
// Struktur für die Anschlussbelegung eines Ultraschallsensors
struct Sensor {
  uint8_t trig;                           // Trigger-Pin
  uint8_t echo;                           // Echo-Pin
};

// Array mit allen Sensoren in Reihenfolge rund um den Teller
Sensor sensoren[6] = {
  {43, 42}, {45, 44}, {47, 46}, {48, 49}, {50, 51}, {41,40}
};



void setup() {
  Serial.begin(9600);                      // Serielle Ausgaben
  Serial2.begin(9600);                     // Verbindung zum Master

  pinMode(STOPP, INPUT_PULLUP);            // Endschalter
  attachInterrupt(digitalPinToInterrupt(STOPP), LowerStepper::stopInterrupt, FALLING); // Interrupt bei Betätigung

  
  for(int i = 0; i < 6; i++) {              // Sensorpins konfigurieren
    pinMode(sensoren[i].trig, OUTPUT);
    pinMode(sensoren[i].echo, INPUT);
  }
  
  stepper2.begin();                         // oberen Stepper initialisieren
  stepper2.test();                          // kurze Funktionsprüfung
  
  stepper.begin();                          // unteren Stepper initialisieren
  gsm.begin();                              // Gleichstrommotor initialisieren
  gsm.disable();                           // vorerst ausschalten
}

#include "Messager.h"

Modus m = OFF;       // aktueller Betriebsmodus

uint8_t stufe = 0;   // aktuelle Lüfterstufe
uint8_t m_pos = 0;   // Position des oberen Motors

//=================================================================
//=================================================================
//=================================================================
//=================================================================
#define DEV

bool kalibiert = false;
long r0[6], r30[6];

void loop() {
  /*
  while(true) {
    if(Serial.available() > 0) {
      String input = Serial.readStringUntil("\n");
      input.trim();
      if(input.equals("ON")) {
        gsm.enable();
      } else if(input.equals("OFF")) {
        gsm.disable();
      } else {
        int deg = input.toInt();
        Serial.println(deg);
        stepper2.movestepper(deg);
      }
      
    }
    
    gsm.setSpeed(64);
  }
  */
  if(kalibiert == false) {                // Einmalige Kalibrierung
    kalibiere(7); // Messanzahl übergeben
    kalibiert = true;
  }
  getCurrentData();


  if(m == ON) {
    stepper.getStepper().enable();
    stepper2.getStepper().enable();
    
    auswertenUndDrehen();
    
    gsm.enable();
    int speed = map(stufe, 0, 4, 0, 255);
    gsm.setSpeed(speed);

  } else if(m == OFF) {
    gsm.disable();
    stepper.getStepper().disable();
    stepper2.getStepper().disable();
  } else if(m == MANUAL) {
    gsm.enable();
    int speed = map(stufe, 0, 4, 0, 255);
    gsm.setSpeed(speed);
    stepper2.movestepper(m_pos*10);
  }
}

// liest das aktuelle Steuerpaket vom Master ein
void getCurrentData() {
  Modus t_m = OFF;       // temporärer Modus
  uint8_t t_stufe = 0;   // temporäre Stufe
  uint8_t t_pos = 0;     // temporäre Position

  static Modus l_m = OFF;       // zuletzt empfangener Modus
  static uint8_t l_stufe = 0;   // zuletzt empfangene Stufe
  static uint8_t l_pos = 0;     // zuletzt empfangene Position

  if(!Messager.available()) {             // warten bis drei Bytes da sind
    delay(100);
    return;                   // kein neues Paket -> Funktion beenden
  }
  Messager.read(t_m, t_stufe, t_pos);


  if(t_stufe > 4)   t_stufe = 4;
  if(t_pos > 36)  t_pos   = 36;

  // nur bei Änderung Werte übernehmen
  if(t_m != l_m || t_stufe!= l_stufe || t_pos  != l_pos) {
    l_m = t_m;
    l_stufe = t_stufe;
    l_pos = t_pos;

    m = l_m;
    stufe = l_stufe;
    m_pos = l_pos;

    //#ifdef DEV
    Serial.print(m);
    Serial.print(",");
    Serial.print(stufe);
    Serial.print(",");
    Serial.print(m_pos);
    Serial.println(",");
    //#endif
  }
}


// einfache Entfernungsmessung mit Ultraschallsensor
long messung(uint8_t trigPin, uint8_t echoPin, const int MAX_ENTFERNUNG) {
  digitalWrite(trigPin, LOW);              // Impuls vorbereiten
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);             // Trigger senden
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long dauer = pulseIn(echoPin, HIGH);     // Echo-Zeit messen
  long entfernung = dauer/ 2 * 0.03432;    // Entfernung in cm berechnen
  if(entfernung >= MAX_ENTFERNUNG) return MAX_ENTFERNUNG;
  delay(50);
  return entfernung;
}










