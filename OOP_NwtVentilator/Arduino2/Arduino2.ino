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


//#define DEV

#include "DcMotor.h"
DcMotor gsm(10, 11, 12, 13, 9);

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
  gsm.begin();
  gsm.disable();
}

#include "Messager.h"

Modus m = OFF;

uint8_t stufe = 0;
uint8_t m_pos = 0;

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
  if(kalibiert == false) {
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

void getCurrentData() {
  Modus t_m = OFF;
  uint8_t t_stufe = 0;
  uint8_t t_pos = 0;

  static Modus l_m = OFF;
  static uint8_t l_stufe = 0;
  static uint8_t l_pos = 0;

  if(!Messager.available()) {
    delay(100);
    return;                   // kein neues Paket -> Funktion beenden
  }
  Messager.read(t_m, t_stufe, t_pos);


  if(t_stufe > 4)   t_stufe = 4;
  if(t_pos > 36)  t_pos   = 36;

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


long messung(uint8_t trigPin, uint8_t echoPin, const int MAX_ENTFERNUNG) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long dauer = pulseIn(echoPin, HIGH);
  long entfernung = dauer/ 2 * 0.03432;
  if(entfernung >= MAX_ENTFERNUNG) return MAX_ENTFERNUNG;
  delay(50);
  return entfernung;
}










