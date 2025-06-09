// Klasse zur Steuerung der dreifarbigen Ampel-LED.
class AmpelDisplay {                      // Klasse für die dreifarbige LED-Anzeige
  private:
    // Pins der einzelnen Farben merken
    short int r;                          // Pin für Rot
    short int g;                          // Pin für Grün
    short int y;                          // Pin für Gelb

  public:
    // Pins über den Konstruktor übernehmen
    AmpelDisplay(const int r, const int y, const int g) {
      this->r = r;                        // Rot-Pin speichern
      this->g = g;                        // Grün-Pin speichern
      this->y = y;                        // Gelb-Pin speichern
    }

  public:
    // Pins als Ausgänge einrichten
    void begin() {
      pinMode(r, OUTPUT);                 // Rot-Pin als Ausgang
      pinMode(g, OUTPUT);                 // Grün-Pin als Ausgang
      pinMode(y, OUTPUT);                 // Gelb-Pin als Ausgang
    }

    // Grünes Licht: Automatik aktiv
    void Automatic() {
      digitalWrite(r, LOW);               // Rot aus
      digitalWrite(g, HIGH);              // Grün an
      digitalWrite(y, LOW);               // Gelb aus
    }

    // Rotes Licht: Gerät aus
    void Off() {
      digitalWrite(this->r, HIGH);        // Rot an
      digitalWrite(this->g, LOW);         // Grün aus
      digitalWrite(this->y, LOW);         // Gelb aus
    }

    // Gelbes Licht: manueller Modus
    void Manual() {
      digitalWrite(this->r, LOW);         // Rot aus
      digitalWrite(this->g, LOW);         // Grün aus
      digitalWrite(this->y, HIGH);        // Gelb an
    }
};
