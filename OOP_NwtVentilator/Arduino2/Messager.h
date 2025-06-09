// Serielle Kommunikationsschnittstelle für Arduino 2.
#include "HardwareSerial.h"                           //Serielle Schnittstelle nutzen
#include <stdint.h>

//Aufzählung der verfügbaren Betriebsmodi
enum Modus {OFF = 0, ON = 1, MANUAL = 2};

//Klasse kümmert sich um die serielle Kommunikation zwischen den Arduinos
class MessagerClass {
  private:
    //Referenz auf die verwendete serielle Schnittstelle
    HardwareSerial &serialPort;

  public:
    //Konstruktor übernimmt die gewählte Schnittstelle und initialisiert sie
    MessagerClass(HardwareSerial &port, unsigned long baud): serialPort(port) {
      serialPort.begin(baud);               //Baudrate setzen
    }

    //Datenpaket senden (Modus, Geschwindigkeit, Position)
    void send(Modus m, uint8_t speed, uint8_t pos) {
      this->serialPort.write((uint8_t)m);   //Modus übertragen
      this->serialPort.write(speed);        //Geschwindigkeit übertragen
      this->serialPort.write(pos);          //Position übertragen
    }

    //Prüfen ob ein komplettes Paket empfangen wurde
    bool available() {
      return serialPort.available() >=3;    //drei Bytes müssen bereitstehen
    }

    //Empfängt ein Paket und legt die Werte in den übergebenen Variablen ab
    void read(Modus &outMode, uint8_t &outSpeed, uint8_t &outpos) {
      if(available()) {
        outMode  = static_cast<Modus>(serialPort.read()); //Modus auslesen
        outSpeed = serialPort.read();                    //Geschwindigkeit auslesen
        outpos   = serialPort.read();                    //Position auslesen
      }
    }
};

//Globales Objekt für die Kommunikation auf Serial2
MessagerClass Messager(Serial2, 9600);