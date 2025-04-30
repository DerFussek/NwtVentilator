//===================================================//
//                   Einstellungen                   //
//===================================================//

//*****************Fernbedinung**********************//
#include <IRremote.h>
const short int remotePIN = 22;

IRrecv remote(remotePIN);

//*****************Ampelanzeige**********************//
const short int trafficlightRED = 12;
const short int trafficlightYELLOW = 11;
const short int trafficlightGREEN = 13;

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

A4988 stepper1(STEPS, s1_DIR, s1_STEP, s1_SLEEP, s1_MS1, s1_MS2, s1_MS3);

//Schrittmotor 2 (Pinbelegungnen für den zweiten Schrittmotor)
const short int s1_DIR = 2;
const short int s1_STEP = 3;
const short int s1_SLEEP = 4;
const short int s1_MS1 = 7;
const short int s1_MS2 = 6;
const short int s1_MS3 = 5; 

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
//               Variablen & Objekte                 //
//===================================================//

//Fernbedinung


//Statusinformation
bool automatic = false;
bool off = true;
bool manual = false;


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
}

void loop() {
  readRemote();
}

void readRemote() {
  static long pressDelay = millis();

  if(!remote.decode() || !(millis() - pressDelay >= 150)) return; //Wenn der Empfänger nichts empfangen hat oder noch keine 150ms 
                                                                  //nach dem letzen knopfdruck vergangen sind, kann/wird der Empänger nicht ausgelesen
  
  unsigned long command = remote.decodedIRData.command; //Ausgabe des Empfängers wird in der Variable command gespeichert(in HEX).


  /*
    In der folgenden If-Else Abfolge wird überprüft welche Taste auf der Fernbedinung gedrückt wurde. 
    Dabei hat jede taste einen individuellen HEX-Code den es nur einmal gibt.
  */
  if(command == 0x45) { //Taste "A"
    TrafficlightAUTOMATISCH();
    automatic = true;
    manual = off = false;
  } else if(command == 0x47) { //Taste "B"
    TrafficlightOFF();
    off = true;
    manual = automatic = false;
  } else if(command == 0x09) { //Taste "C"
    TrafficlightMANUAL();
    manual = true;
    automatic = off = false;
  }


  remote.resume(); //Empänger wartet auf das nächste Signal
  pressDelay = millis(); //Delay wird zurückgesetzt
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