//===================================================//
//                   Einstellungen                   //
//===================================================//

//*****************Fernbedinung**********************//
#include <IRremote.h>
const short int remotePIN = 13;


IRrecv remote(remotePIN);

//*****************Ampelanzeige**********************//
const short int trafficlightRED = 9;
const short int trafficlightYELLOW = 10;
const short int trafficlightGREEN = 11;

//*******************Ringled************************//
#include <Adafruit_NeoPixel.h>
const short int ringLED = 12;  //Pin an dem die Ringled angeschlossen ist
const short int numberLEDS = 8;

int ringLedColor[3] = {255, 0, 0};

Adafruit_NeoPixel ring(numberLEDS, ringLED, NEO_GRB + NEO_KHZ800);

//*****************Schrittmotoren*********************//
//Generelle Konfigurationen (für beide Motoren gleich)
#include "A4988.h"

const short int STEPS = 200;     
const short int microSTEPS = 16;
const short int RPM = 120;

//Schrittmotor 1 (Pinbelegungnen für den ersten Schrittmotor)
const short int s1_DIR = 2;
const short int s1_STEP = 3;
const short int s1_SLEEP = 4;
const short int s1_MS1 = 7;
const short int s1_MS2 = 6;
const short int s1_MS3 = 5; 

const short int s1_STOPP;

A4988 stepper1(STEPS, s1_DIR, s1_STEP, s1_SLEEP, s1_MS1, s1_MS2, s1_MS3);

//Schrittmotor 2 (Pinbelegungnen für den zweiten Schrittmotor)
const short int s2_DIR = 2;
const short int s2_STEP = 3;
const short int s2_SLEEP = 4;
const short int s2_MS1 = 7;
const short int s2_MS2 = 6;
const short int s2_MS3 = 5; 

A4988 stepper2(STEPS, s2_DIR, s2_STEP, s2_SLEEP, s2_MS1, s2_MS2, s2_MS3);

//***************Gleichschrittmotor*******************//
const short int gsm_IN1;
const short int gsm_IN2;
const short int gsm_IN3;
const short int gsm_IN4;

const short int gsm_SPEED;

//***************Ultraschallsensoren*******************//
//TODO

//***************Oled-Display*******************//
//TODO

//===================================================//
//           Projekt Variablen & Objekte             //
//===================================================//

//Statusinformation
bool automatic = false;
bool off = true;
bool manual = false;

unsigned short int stufe = 1;


//===================================================//
//                   Programmstart                   //
//===================================================//
void setup() {
  Serial.begin(9600); //Serieller Monitor starten
  
  remote.enableIRIn(); //IR-Fernbedinung starten;
  
  //Ampel-Display-Pins auf Ausgang setzen
  pinMode(trafficlightRED, OUTPUT);
  pinMode(trafficlightYELLOW, OUTPUT);
  pinMode(trafficlightGREEN, OUTPUT);

  //Ring-LED
  ring.begin();
  ring.show();
}

void loop() {
  readRemote();
  LedRing();
  Serial.println(stufe);
}

void readRemote() {
  static long pressDelay = millis();

  if(remote.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) return;
  if(!remote.decode() || !(millis() - pressDelay >= 500)) return; //Wenn der Empfänger nichts empfangen hat oder noch keine 150ms 
  

  pressDelay = millis(); //Delay wird zurückgesetzt                                                                 //nach dem letzen knopfdruck vergangen sind, kann/wird der Empänger nicht ausgelesen
  
  unsigned long command = remote.decodedIRData.command; //Ausgabe des Empfängers wird in der Variable command gespeichert(in HEX).


  /*
    In der folgenden If-Else Abfolge wird überprüft welche Taste auf der Fernbedinung gedrückt wurde. 
    Dabei hat jede taste einen individuellen HEX-Code den es nur einmal gibt.
  */
  if(command == 0x45) { //Taste "A"
    TrafficlightAUTOMATISCH();
    automatic = true;
    manual = off = false;

    ringLedColor[0] = 0;
    ringLedColor[1] = 128;
    ringLedColor[2] = 0;
  } else if(command == 0x47) { //Taste "B"
    TrafficlightOFF();
    off = true;
    manual = automatic = false;

    ringLedColor[0] = 128;
    ringLedColor[1] = 0;
    ringLedColor[2] = 0;
  } else if(command == 0x09) { //Taste "C"
    TrafficlightMANUAL();
    manual = true;
    automatic = off = false;

    ringLedColor[0] = 128;
    ringLedColor[1] = 64;
    ringLedColor[2] = 0;
  } else if(command == 0x46) { //Taste "UP"
    stufe++;
  } else if(command == 0x15) { //Taste "DOWN" 
    stufe--;
  }
  command = "";
  
  remote.resume(); //Empänger wartet auf das nächste Signal
   
}

/*
  Die folgenden drei Funktionen geben an Welche Lampen leuchten sollen, wenn der Ventilator An, Aus, oder im manuellen Modus ist. 
*/
void TrafficlightAUTOMATISCH() {
  digitalWrite(trafficlightGREEN, HIGH);
  digitalWrite(trafficlightRED, LOW);
  digitalWrite(trafficlightYELLOW, LOW);
}

void TrafficlightOFF() {
  digitalWrite(trafficlightGREEN, LOW);
  digitalWrite(trafficlightRED, HIGH);
  digitalWrite(trafficlightYELLOW, LOW);
}

void TrafficlightMANUAL() {
  digitalWrite(trafficlightGREEN, LOW);
  digitalWrite(trafficlightRED, LOW);
  digitalWrite(trafficlightYELLOW, HIGH);
}

void LedRing() {
    static long pressDelay = millis();
    if(!(millis() - pressDelay >= 200)) return;
    pressDelay = millis();

    ring.clear();
    int ledProStufe = numberLEDS / 6;

    for(int i = 0; i < stufe * ledProStufe; i++) {
      ring.setPixelColor(i, ring.Color(ringLedColor[0], ringLedColor[1], ringLedColor[2]));
    } 

    
    ring.show();
}