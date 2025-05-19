#include "Arduino.h"
#include "A4988.h"
#include "LowerStepper.h"
#define MOTOR_STEPS 200
#define MICROSTEPS 16
#define RPM 20

#define DIR 13
#define STEP 12
#define SLEEP 11
#define MS1 8
#define MS2 9
#define MS3 10

#define STOPP 21  //Pin 21 hat die höchste Interrupt-Priorität auf dem Arduino-Mega

LowerStepper stepper(RPM, DIR, STEP, SLEEP, MS1, MS2, MS3, STOPP);

#include "UpperStepper.h"

#define upper_DIR 2
#define upper_STEP 3
#define upper_SLEEP 4
#define upper_MS3 7
#define upper_MS2 6
#define upper_MS1 5

//UpperStepper stepper2(RPM, upper_DIR, upper_STEP, upper_SLEEP, upper_MS1, upper_MS2, upper_MS3);

#include "GSM.h"
//GSM gsm(12, 11, 10, 9, 8);

//=========US-Sensoren=========//
struct Sensor {
  uint8_t trig;
  uint8_t echo;
};

Sensor sensoren[6] = {{22,23}, {24,25}, {26,27}, {28,29}, {30,31}, {32,33}};
long messwerte[12];
int winkel = 0;
const int MAX_DISTANZ = 80; //[in cm]


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
  if(tempModus != lastModus && tempModus != -1) {
    lastModus = tempModus;
    m = lastModus;
    Serial.print("Modus: ");
    Serial.println(m);
  }

  
  if(m == OFF) {  
    on = false;
    manuel = false;
    
    off = true;
    stufe = 0;
    
  } else if(m == ON) {
    off = false;
    manuel = false;

    on = true;
    stufe = 1;
  } else if(m == MANUAL) {
    on = false;
    off = false;

    manuel = true;
    stufe = 1;
  } 
  delay(250);

  if(on) {
    winkel = 0;

    for (int s = 0; s < 6; s++) {
      messwerte[winkel++] = gefilterteMessung(sensoren[s].trig, sensoren[s].echo);
      delay(50);
    }

    delay(500);
    stepper.move(120);

    Serial.println("Messungen:");
    for (int s = 0; s < 6; s++) {
      messwerte[winkel++] = gefilterteMessung(sensoren[s].trig, sensoren[s].echo);
      delay(50);
    }

    for(int i=0; i<12; i++) {
      if(i==6) Serial.println("2.Druchgang:");
      Serial.println(messwerte[i]);
    }
    delay(500);
    stepper.move(-120);

    int minIndex = -1;
    for (int i = 0; i < 12; i++) {
      if (messwerte[i] >= 0 && messwerte[i] <= MAX_DISTANZ) {
        if (minIndex == -1 || messwerte[i] < messwerte[minIndex]) {
          minIndex = i;
        }
      }
    }

    if (minIndex == -1) {
      Serial.println("Kein Hindernis erkannt");
    } else {
      int richtung = minIndex * 30;
      Serial.print("Hindernis in Richtung: ");
      Serial.print(richtung);
      Serial.println("°");
    }
    
    //stepper.stepperToReferencePoint();
    delay(2000);

  } else if(off) {

  } else if(manuel) {

  } else {

  }
  
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
  return (d <= MAX_DISTANZ) ? d : -1;
}


