/*
 * Hauptsketch der klassischen Ventilatorsteuerung.
 * Liest die Fernbedienung aus und steuert LEDs und Motoren.
 */
//===================================================//
//                   Einstellungen                   //
//===================================================//

//*****************Fernbedinung**********************//
#include <IRremote.h>
const short int remotePIN = 13;      // Pin für den IR-Empfänger

// Objekt zum Einlesen der Fernbedienung
IRrecv remote(remotePIN);

//*****************Ampelanzeige**********************//
const short int trafficlightRED = 9;
const short int trafficlightYELLOW = 10;
const short int trafficlightGREEN = 11;

//*******************Ringled************************//
#include <Adafruit_NeoPixel.h>
const short int ringLED = 12;  // Pin an dem die Ringled angeschlossen ist
const short int numberLEDS = 8; // Anzahl der LEDs im Ring

// Aktuelle Farbe des LED-Rings (RGB)
int ringLedColor[3] = {255, 0, 0};

// Objekt zur Ansteuerung des Rings
Adafruit_NeoPixel ring(numberLEDS, ringLED, NEO_GRB + NEO_KHZ800);

//*****************Schrittmotoren*********************//
//Generelle Konfigurationen (für beide Motoren gleich)
#include "A4988.h"

const short int STEPS = 200;   // Vollschritte pro Umdrehung
const short int microSTEPS = 16; // Mikroschritte
const short int RPM = 120;     // Drehzahl in U/min

//Schrittmotor 1 (Pinbelegungnen für den ersten Schrittmotor)
const short int s1_DIR = 2;    // Richtungspin
const short int s1_STEP = 3;   // Schrittpin
const short int s1_SLEEP = 4;  // Schlafmodus
const short int s1_MS1 = 7;    // Microstep 1
const short int s1_MS2 = 6;    // Microstep 2
const short int s1_MS3 = 5;    // Microstep 3

const short int s1_STOPP;  // Endschalter für Motor 1

A4988 stepper1(STEPS, s1_DIR, s1_STEP, s1_SLEEP, s1_MS1, s1_MS2, s1_MS3);

//Schrittmotor 2 (Pinbelegungnen für den zweiten Schrittmotor)
const short int s2_DIR = 2;    // Richtungspin
const short int s2_STEP = 3;   // Schrittpin
const short int s2_SLEEP = 4;  // Schlafmodus
const short int s2_MS1 = 7;    // Microstep 1
const short int s2_MS2 = 6;    // Microstep 2
const short int s2_MS3 = 5;    // Microstep 3

A4988 stepper2(STEPS, s2_DIR, s2_STEP, s2_SLEEP, s2_MS1, s2_MS2, s2_MS3);

//***************Gleichschrittmotor*******************//
const short int gsm_IN1;       // Steuerpin 1 des DC-Motors
const short int gsm_IN2;       // Steuerpin 2
const short int gsm_IN3;       // Steuerpin 3
const short int gsm_IN4;       // Steuerpin 4

const short int gsm_SPEED;     // PWM-Pin für Drehzahl

//***************Ultraschallsensoren*******************//
//TODO

//***************Oled-Display*******************//
//TODO

//===================================================//
//           Projekt Variablen & Objekte             //
//===================================================//

//Statusinformation
bool automatic = false;  // Automatikmodus aktiv?
bool off = true;  // Alle Funktionen ausgeschaltet?
bool manual = false;  // Manueller Modus aktiv?

unsigned short int stufe = 1;  // Aktuelle Lüfterstufe


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

// Liest ein IR-Signal ein und aktualisiert den Betriebsmodus
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
  
  remote.resume();            // Empfänger wartet auf das nächste Signal
   
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
// Aktualisiert die Anzeige des LED-Rings entsprechend der Stufe

void LedRing() {
    static long pressDelay = millis();          // Zeitstempel für Updates
    if(!(millis() - pressDelay >= 200)) return; // nur alle 200 ms aktualisieren
    pressDelay = millis();

    ring.clear();                               // LEDs ausschalten
    int ledProStufe = numberLEDS / 6;           // LEDs pro Stufe

    // entsprechend der aktuellen Stufe einschalten
    for(int i = 0; i < stufe * ledProStufe; i++) {
      ring.setPixelColor(i, ring.Color(ringLedColor[0], ringLedColor[1], ringLedColor[2]));
    }

    ring.show();
}
