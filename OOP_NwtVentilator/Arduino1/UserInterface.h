#pragma once
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <Adafruit_ST7789.h> 
#include <SPI.h>
#include <Wire.h>
#include <DS3231.h>

class UserInterface {
  private:
    Adafruit_ST7735 tft;
    
    DS3231 myRTC;
    bool h12, PM, century;
    String wochentag[7] {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};

  public:
    UserInterface(const int CS, const int DC, const int MOSI, const int SCLK, const int RST) : tft(CS, DC, MOSI, SCLK, RST) {

    }

    void begin() {
      this->tft.initR(INITR_GREENTAB);
      drawStaticUserInterface();
    }

    void drawArc(int x, int y, int r, int startAngle, int endAngle, int thickness, uint16_t color) {
      for (int angle = startAngle - 90; angle <= endAngle - 90; angle++) {
        float rad = angle * 3.14159 / 180.0;
        for (int t = 0; t < thickness; t++) {
          int px = x + (r - t) * cos(rad);
          int py = y + (r - t) * sin(rad);
          tft.drawPixel(px, py, color);
        }
      }
    }

    void drawStaticUserInterface() {
      tft.fillScreen(ST77XX_BLACK); //Displayinhalt löschen

      tft.setTextColor(tft.color565(0, 255, 165));
      tft.setTextSize(4);

      //Uhrzeit berechnen & anzeigen
      byte hour = myRTC.getHour(h12, PM); //aktuelle Uhzeit hohlen
      byte minute = myRTC.getMinute();
      String uhrTxt = (hour < 10 ? "0" : "") + String(hour) + ":" +   //aktuelle uhrzeit als String speichern
                      (minute < 10 ? "0" : "") + String(minute);
      
      const char* uhrTxtC = uhrTxt.c_str(); //Arduino-String wird in C-String umgewandelt

      int16_t _uhrX, _uhrY; //soll x + y Position des textes speichern
      uint16_t uhrW, uhrH;  //soll breite und höhe des textes
      
      tft.getTextBounds(uhrTxtC, 0, 0, &_uhrX, &_uhrY, &uhrW, &uhrH); //hier wird die breite, höhe und x + y position 
                                                                      //der linkeren oberen ecke des Tetxes in verschiedenen Variablen gespeichert
      
      int16_t uhrX = (128 - uhrW) / 2 - _uhrX + 3;    //x Position ausrechen, an dem der Text zentriert ist
      int16_t uhrY = (160 - uhrH) / 2 - _uhrY + 26;   //y Position ausrechen, an dem der Text zentriert ist und von der Mitte um 26 pixel nach unten verschieben
      
      tft.setCursor(uhrX, uhrY);  //Uhrzeit auf Display zeichnen
      tft.print(uhrTxtC);         //



      // Datum anzeigen
      byte day = myRTC.getDate();           //Aktueller Tag, Monat und Jahr in Variablen speichern
      byte month = myRTC.getMonth(century); // 
      byte year = myRTC.getYear();          //
      String dateTxt = (day < 10 ? "0" : "") + String(day) + "." +                          //
                      (month < 10 ? "0" : "") + String(month) + "." + "20" + String(year); //Datum zusammengesetzt als String speichern
      
      const char* dateTxtC = dateTxt.c_str();   //Datum als C-String speichern                                         

      tft.setTextColor(tft.color565(255, 255, 255));  //Textfarbe auf Weiß setzen
      tft.setTextSize(1); //Größe des textes festlegen

      int16_t _dateX, _dateY; //soll x + y Position des textes speichern
      uint16_t dateW, dateH;  //soll breite und höhe des textes
      
      tft.getTextBounds(dateTxtC, 0, 0, &_dateX, &_dateY, &dateW, &dateH); //hier wird die breite, höhe und x + y position 
                                                                          //der linkeren oberen ecke des Tetxes in verschiedenen Variablen gespeichert
      
      int16_t dateX = (128 - dateW) / 2 - _dateX; //Mitte herausfinden
      int16_t dateY = uhrY + uhrH + 5;  //Position 5px unter der Uhrzeit 
      
      tft.setCursor(dateX, dateY);  //Datum auf Display zeichen
      tft.print(dateTxtC);          //



      //Wochentag
      tft.setTextColor(tft.color565(255, 255, 255));  //Textfarbe festlegen
      tft.setTextSize(1);                             //Textgröße festlegen
      
      String DowTxt = wochentag[myRTC.getDoW()]; //aktuellen Wochentag abfragen und dessen abkürzung in einem String speichern

      int16_t _DowX, _DowY;
      uint16_t DowW, DowH;

      const char* DowTxtC = DowTxt.c_str();

      tft.getTextBounds(DowTxtC, 0, 0, &_DowX, &_DowY, &DowW, &DowH);
      
      int16_t DowX = (128 - DowW) / 2 - _DowX;
      int16_t DowY = uhrY - 15;
      
      tft.setCursor(DowX, DowY);  //
      tft.print(DowTxtC);         //Wochentag auf dem Display anzeigen

      

      //**Kreise**/
      tft.setTextSize(2);
      int16_t circleR = 20;
      
      //Stufe
      tft.fillCircle(32, 38, circleR, tft.color565(3, 26, 12));   //dunkelgrüner Kreis mit einem Radius von 20px zeichen
      tft.fillCircle(32, 38, circleR - 3, tft.color565(0, 0, 0)); //Schwarzen Kreis mit einem Radius von 17px an der gleichen stelle zeichnen,
                                                                  //um einen Ring von 3px zu zeichenn
      
      drawArc(32, 38, circleR, 0, 180, 3, tft.color565(0, 255, 165)); //Laufring zeichnen
      
      tft.setCursor(27, 32);  //Text mittig im Kreis ausrichten
      tft.println("0"); //Stufe zeichen

      tft.setTextColor(ST77XX_WHITE); //Text "Stufe" unter dem Kreis zeichen
      tft.setTextSize(1);             //
      tft.setCursor(16, 62);          //
      tft.print("Stufe");             //

      // Rechts
      tft.fillCircle(96, 38, circleR, tft.color565(0, 25, 50));   //dunkelorangen Kreis mit einem Radius von 20px zeichen
      tft.fillCircle(96, 38, circleR - 3, tft.color565(0, 0, 0)); //Schwarzen Kreis mit einem Radius von 17px an der gleichen stelle zeichnen,
                                                                  //um einen Ring von 3px zu zeichenn
      
      drawArc(96, 38, circleR, 0, 250, 3, tft.color565(0, 128, 255)); //Laufring zeichnen
      
      tft.setCursor(86, 32); //Text mittig im Kreis ausrichten
      tft.println("00"); //Platzhalter für die Temperatur zeichnen (später Variable)
      
      tft.setTextColor(ST77XX_WHITE); //Text "Temp." unter dem Kreis zeichen
      tft.setTextSize(1);             //
      tft.setCursor(84, 62);          //
      tft.print("Temp.");             //
    }

    void updateDynamicUserInterface(int delayMs, int level) {
      static String lastTime = "";
      static byte lastTemp = "";
      static int32_t lastDelay = millis();
      static int lastLevel = level;
      const int16_t circleR = 20;

      if(!(millis() - lastDelay) >= delayMs) return;

      byte hour = myRTC.getHour(h12, PM);   //Aktuelle Stunde abfragen
      byte minute = myRTC.getMinute();  //Aktuelle Minute abfragen
      byte temp = myRTC.getTemperature(); //Aktuelle Temperatur abfragen

      String uhrTxt = (hour < 10 ? "0" : "") + String(hour) + ":" +
                      (minute < 10 ? "0" : "") + String(minute);

      if(uhrTxt != lastTime) {
        lastTime = uhrTxt;

        // Hintergrundfläche für Uhrzeit löschen
        tft.fillRect(0, 85, 128, 35, ST77XX_BLACK);

        tft.setTextColor(tft.color565(0, 255, 165));
        tft.setTextSize(4);

        const char* uhrTxtC = uhrTxt.c_str();
        int16_t _uhrX, _uhrY;
        uint16_t uhrW, uhrH;
        tft.getTextBounds(uhrTxtC, 0, 0, &_uhrX, &_uhrY, &uhrW, &uhrH);
        int16_t uhrX = (128 - uhrW) / 2 - _uhrX;
        int16_t uhrY = (160 - uhrH) / 2 - _uhrY + 26;
        tft.setCursor(uhrX, uhrY);
        tft.print(uhrTxtC);

        if(abs(temp - lastTemp) >= 1) {
          lastTemp = temp;
          float minTemp = 0;
          float maxTemp = 40;
          float winkelF = map(temp, minTemp, maxTemp, 0, 360);
          int winkel = constrain(round(winkelF), 0, 360);

          tft.fillRect(76, 18, 2*20, 2*20, ST7735_BLACK);
          tft.fillCircle(96, 38, 20, tft.color565(0, 25, 50));
          tft.fillCircle(96, 38, 20 - 3, tft.color565(0, 0, 0));
          drawArc(96, 38, 20, 0, winkel, 3, tft.color565(0, 128, 255));
          tft.setTextSize(2);
          tft.setTextColor(tft.color565(255, 255, 255));
          tft.setCursor(86, 32);
          tft.println(temp);
        } 
      } 
      
      if(level != lastLevel) {
        lastLevel = level;
        float winkelF = map(level, 0, 4, 0, 360);
        int winkel = constrain(round(winkelF), 0, 360);

        tft.fillRect(12, 18, 2*20, 2*20, ST7735_BLACK);
        tft.fillCircle(32, 38, circleR, tft.color565(3, 26, 12)); 
        tft.fillCircle(32, 38, circleR - 3, tft.color565(0, 0, 0));
        drawArc(32, 38, circleR, 0, winkel, 3, tft.color565(0, 255, 165));

        tft.setTextSize(2);
        tft.setTextColor(tft.color565(255, 255, 255));
        tft.setCursor(27, 32);  //Text mittig im Kreis ausrichten
        tft.println(level);
      }

      lastDelay = millis();
    }
};