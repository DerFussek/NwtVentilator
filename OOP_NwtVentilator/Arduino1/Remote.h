// Klasse zum Auswerten der Infrarot-Fernbedienung.
#include <IRremote.h>

enum Button { NONE, A, B, C, UP, DOWN, RIGHT, LEFT }; //Datentyp für die Tasten der Fernbedinung erstellen

class Remote {  //Neue Klasse für die Fernbedinung erstellen
  private:
    short int pin;  //Pin an der die Fernbedinung an geschlossen ist
    IRrecv remote;  //Neues Objekt der Klasse IRrecv erstellen
  
  public:
    Remote(const int pin) : remote(pin) { //Atribbute über Konstruktor übergeben
      this->pin = pin;
    }

  public:
    /*Sensor starten*/
    void start() { 
      remote.enableIRIn();
    }
    
    /*
      Funktion fragt den Sensor ab und gibt den gedrückten Knopf zurück.
      Außerdem darf die Taste nur nach einer Zeit(pressDelayMs) gedrückt werden
    */
    Button awaitInput(int pressDelayMs) {
      static long pressDelay = millis();

      if (!remote.decode()) return NONE;  //wenn der Empfänger nichts empfängt -> verlasse Funktion

      if (remote.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
          remote.resume();
          return NONE; //wenn der Empfänger die gleiche Taste nochmal empfangen hat -> verlasse Funktion
      } 
      
      if (millis() - pressDelay < pressDelayMs) return NONE;  //wenn der Timer nicht abgelaufen ist -> verlasse Funktion
      pressDelay = millis();  //sete Timer zurück                     

      unsigned long command = remote.decodedIRData.command; //Tastencode vom Empfänger einlesen
      remote.resume();  //Empfänger wieder starten

      switch (command) {  //Für die gedrückte Taste den passenden Wert zurückgeben
        case 0x45: return A;      //Taste "A"
        case 0x47: return B;      //Taste "B"
        case 0x09: return C;      //Taste "C"
        case 0x46: return UP;     //Taste "UP"
        case 0x15: return DOWN;   //Taste "DOWN"
        case 0x43: return RIGHT;  //Taste "RIGHT"
        case 0x44: return LEFT;   //Taste "LEFT"
        default:    return NONE;  //keine oder unbekannte Taste gedrückt
      }
    }
};