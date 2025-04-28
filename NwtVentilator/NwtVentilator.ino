
//Fernbedinung
#include <IRremote.h>
const short remotePIN = 22;
IRrecv remote(remotePIN);

//AMPEL DISPLAY
const short trafficlightRED = 12;
const short trafficlightYELLOW = 11;
const short trafficlightGREEN = 13;


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
    TrafficlightON();
  } else if(command == 0x47) { //Taste "B"
    TrafficlightOFF();
  } else if(command == 0x09) { //Taste "C"
    TrafficlightMANUAL();
  }


  remote.resume(); //Empänger wartet auf das nächste Signal
  pressDelay = millis(); //Delay wird zurückgesetzt
}

/*
  Die folgenden drei Funktionen geben an Welche Lampen leuchten sollen, wenn der Ventilator An, Aus, oder im manuellen Modus ist. 
*/
void TrafficlightON() {
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