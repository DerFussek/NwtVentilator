class AmpelDisplay {
  private:
    //Klassenvariablen
    short int r;
    short int g;
    short int y;

  public: 
    AmpelDisplay(const int r, const int y, const int g) { //Parameter
      this->r = r;
      this->g = g;
      this->y = y; 
    }

  public:
    void begin() {
      pinMode(r, OUTPUT);
      pinMode(g, OUTPUT);
      pinMode(y, OUTPUT);
    }

    //Gruen
    void Automatic() {
      digitalWrite(r, LOW);
      digitalWrite(g, HIGH);
      digitalWrite(y, LOW);
    }

    //Rote
    void Off() {
      digitalWrite(this->r, HIGH);
      digitalWrite(this->g, LOW);
      digitalWrite(this->y, LOW);
    }

    //Gelbe
    void Manual() {
      digitalWrite(this->r, LOW);
      digitalWrite(this->g, LOW);
      digitalWrite(this->y, HIGH);
    }
};