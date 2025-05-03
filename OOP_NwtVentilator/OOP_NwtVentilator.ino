
//*****************Ampelanzeige**********************//
#include "AmpelDisplay.h"
const int ampelRedPin = 12;
const int ampelYellowPin = 11;
const int ampelGreenPin = 13;

AmpelDisplay ampelDisplay(ampelRedPin, ampelYellowPin, ampelGreenPin);

//*****************Fernbedinung**********************//
#include "Remote.h"
const int remotePIN = 22;

Remote remote(remotePIN);

//*******************Ledstrip************************//
#include "Ledstrip.h"
const int stripPin = 6;
const int stripLedNumber = 8;
const int numberOfLevels = 4;

Ledstrip ledStrip(stripPin, stripLedNumber, numberOfLevels);

//*****************Schrittmotoren*********************//
#include "Nema17.h"
//Schrittmotor 1 (Pinbelegungnen für den ersten Schrittmotor)
const short int s1_DIR = 2;
const short int s1_STEP = 3;
const short int s1_SLEEP = 4;
const short int s1_MS1 = 7;
const short int s1_MS2 = 6;
const short int s1_MS3 = 5; 
const short int s1_RPM = 80;

const short int s1_STOPP;

Nema17 stepper1(s1_RPM, s1_DIR, s1_STEP, s1_SLEEP, s1_MS1, s1_MS2, s1_MS3, s1_STOPP);

//Schrittmotor 2 (Pinbelegungnen für den zweiten Schrittmotor)
const short int s2_DIR = 2;
const short int s2_STEP = 3;
const short int s2_SLEEP = 4;
const short int s2_MS1 = 7;
const short int s2_MS2 = 6;
const short int s2_MS3 = 5; 
const short int s2_RPM = 80;

Nema17 stepper2(s2_RPM, s2_DIR, s2_STEP, s2_SLEEP, s2_MS1, s2_MS2, s2_MS3, -1);

//***************Gleichschrittmotor*******************//
#include "DcMotor.h"
const short int in1;
const short int in2;
const short int in3;
const short int in4;
const short int speed;

DcMotor gsm(in1, in2, in3, in4, speed);

//***************Ultraschallsensoren*******************//
//TODO

//***************TFT-Display*******************//
//TODO

void setup() {
  ampelDisplay.begin();
  remote.start();
  ledStrip.begin();
  stepper1.begin();
  stepper2.begin();
  gsm.begin();

}

void loop() {

}