#include "Arduino.h"
#include "A4988.h"
#include "LowerStepper.h"
#define MOTOR_STEPS 200
#define MICROSTEPS 16
#define RPM 20

//Pins
#define DIR 8
#define STEP 7
#define SLEEP 6
#define MS1 5
#define MS2 4
#define MS3 3

#define STOPP 2  //2 ist Interruptfähig

LowerStepper stepper(RPM, DIR, STEP, SLEEP, MS1, MS2, MS3, STOPP);

#include "UpperStepper.h"

//Pins
#define upper_DIR 27
#define upper_STEP 26
#define upper_SLEEP 25
#define upper_MS3 24
#define upper_MS2 23
#define upper_MS1 22

UpperStepper stepper2(RPM, upper_DIR, upper_STEP, upper_SLEEP, upper_MS1, upper_MS2, upper_MS3);
int position = 0;


//#include "GSM.h"
//GSM gsm(12, 11, 10, 9, 8);

//=========US-Sensoren=========//
struct Sensor {
  uint8_t trig;
  uint8_t echo;
};

Sensor sensoren[6] = {{43, 42}, {45, 44}, {47, 46}, {48, 49}, {50, 51}, {41,40}};



void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

  pinMode(STOPP, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(STOPP), LowerStepper::stopInterrupt, FALLING);

  
  for(int i = 0; i < 6; i++) {
    pinMode(sensoren[i].trig, OUTPUT);
    pinMode(sensoren[i].echo, INPUT);
  }
  
  stepper2.begin();
  stepper2.test();
  
  stepper.begin();
}

#include "Messager.h"
Modus m = OFF;
Modus lastModus;
Modus tempModus;
bool on = false;
bool off = true;
bool manuel;

uint8_t stufe = 0;
uint8_t manual_pos = 0;


bool eichung = false;
const int SENSOREN = 6;
long filterRaum_0[SENSOREN];
long filterRaum_30[SENSOREN];
void loop() {
  Messager.receiver.read(tempModus, stufe, manual_pos);

  if(manual_pos > 36) manual_pos = 0;

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

  
  if(true) {
      const int SENSOREN = 6;
      const int MAX_Abstand = 200; //in cm

      if(eichung == false) {  
        Serial.println("Eichung starten in");
        for(int i=5; i > 0; i--) {
          Serial.print(i+1);
          Serial.println("s");
        }
        Serial.println("Eichung startet");


        delay(1000);
        long Raum_0[3][SENSOREN];
        long Raum_30[3][SENSOREN];

        for(int n = 0; n < 3; n++) {
          
          // Messung in Startposition (0°)
          for (int i = 0; i < SENSOREN; i++) {
            long var = 0;
            var = messung(sensoren[i].trig, sensoren[i].echo);
            
            if(var >= 150) Raum_0[n][i] = 150;
            else Raum_0[n][i] = var;
            delay(50);

          }

          stepper.move(-120);
          for (int i = 0; i < SENSOREN; i++) {
            long var = 0;
            var = messung(sensoren[i].trig, sensoren[i].echo);
            
            if(var >= 150) Raum_30[n][i] = 150;
            else Raum_30[n][i] = var;
          }

          stepper.move(120);
        }

        

        for(int i = 0; i < SENSOREN; i++) {
          filterRaum_0[i] = mittlererWert(Raum_0[0][i], Raum_0[1][i], Raum_0[2][i]);
          filterRaum_30[i] = mittlererWert(Raum_30[0][i], Raum_30[1][i], Raum_30[2][i]);
        }

        eichung = true;
        Serial.println("Eichung abgeschlossen!!!");

        for(int i=0; i<6; i++) {
          Serial.print("Sensor [" + (String)i + "] = ");
          Serial.print(filterRaum_0[i]);
          Serial.print(" , ");
          Serial.println(filterRaum_30[i]);
        }


        delay(2000);
      }

      //NACH EICHUNG

      long messrunde_0[SENSOREN];
      long messrunde_30[SENSOREN];

      int grad[6] = {0, 60, 120, 180, 240, 300};
      int grad2[6] = {30, 90, 150, 210, 270, 330};

      for (int i = 0; i < SENSOREN; i++) {
        long var = 0;
        var = messung(sensoren[i].trig, sensoren[i].echo);
        
        if(var >= 150) messrunde_0[i] = 150;
        else messrunde_0[i] = var;
        delay(50);
      }

      stepper.move(-120);
      for (int i = 0; i < SENSOREN; i++) {
        long var = 0;
        var = messung(sensoren[i].trig, sensoren[i].echo);
        
        if(var >= 150) messrunde_30[i] = 150;
        else messrunde_30[i] = var;
        delay(50);
      }

      stepper.move(120);

      const int HYSTERESE = 20; //in cm
      int ZielSensor = -1;
      bool versetzt = false;
      for(int i = 0; i < SENSOREN; i++) {
        int delta_0 = abs(filterRaum_0[i]- messrunde_0[i]);
        int delta_30 = abs(filterRaum_0[i]- messrunde_0[i]);

        int maxDelta = max(delta_0, delta_30);

        delta_0 > delta_30 ? versetzt = false: versetzt = true;
        if(maxDelta >= HYSTERESE) {
          ZielSensor = i;
        }
      }
      
      Serial.println("Bei 0°: ");
      for(int i = 0; i<6; i++) {
          Serial.print("Sensor" + (String)i + " =");
          Serial.print(filterRaum_0[i]);
          Serial.print(" -> ");
          Serial.print(messrunde_0[i]);
          Serial.print(" => ");
          Serial.println(abs(filterRaum_0[i] - messrunde_0[i]));
      }

      Serial.println("=========================================");
      Serial.println("Bei 30°: ");
      for(int i = 0; i<6; i++) {
          Serial.print("Sensor" + (String)i + " =");
          Serial.print(filterRaum_30[i]);
          Serial.print(" -> ");
          Serial.print(messrunde_30[i]);
          Serial.print(" => ");
          Serial.println(abs(filterRaum_30[i] - messrunde_30[i]));
      }

      Serial.println("=========================================");  

      if(versetzt) movestepper(grad2[ZielSensor]);
      else if(!versetzt) movestepper(grad[ZielSensor]);

    } else if(off == true) {

    } else if(2 == 1) {
      movestepper((manual_pos*10));
    }   //TEST
    
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
  position = (position + delta);
  if (position < 0) position += 360;  // Immer positiv halten
}



long gefilterteMessung(uint8_t trig, uint8_t echo) {
  long d = messung(trig, echo);
  return d;
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

long mittlererWert(long a, long b, long c) {
  // Prüfe, ob a der mittlere Wert ist
  if ((a >= b && a <= c) || (a <= b && a >= c)) {
    return a;
  }
  // Prüfe, ob b der mittlere Wert ist
  else if ((b >= a && b <= c) || (b <= a && b >= c)) {
    return b;
  }
  // Wenn a und b nicht mittig sind, muss es c sein
  else {
    return c;
  }
}


