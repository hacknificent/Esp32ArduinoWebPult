#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // See http://playground.arduino.cc/Main/I2cScanner how to test for a I2C device.

class nifDisplay {
  private:
    String header;
    String footer;
    //DisplayVars
    bool displayBacklight = true;
    int displayBacklightTime = 250; // Sets how long backlight will work after actions
    int displayBacklightTimer;

  public:
    nifDisplay(int displayBacklightTime) {
      this->displayBacklightTime = displayBacklightTime;
      init();
    }
    void init() {
      lcd.init(); // initialize the lcd
      updateDisplayBacklight();
      startBacklightTimer();
    }
    void startBacklightTimer(){
        this->displayBacklightTimer = displayBacklightTime;
    }
    void updateDisplayBacklight() {
       // Display backlight swither
         if (displayBacklightTimer > 0) {
           if (!displayBacklight) {
             lcd.backlight(); // turn on backlight.
             displayBacklight = true;
           }
           displayBacklightTimer--;
         } else {
           if (displayBacklight) {
             lcd.noBacklight(); // turn off backlight
             displayBacklight = false;
           }

         }
         delay(40);
    }

    void setHeader(String message){
        this->header = message;
        lcd.setCursor(0, 0);
        lcd.print(message);// print static message
        startBacklightTimer();
        delay(80);
    }

    void clear(){
        lcd.clear();
    }

};
