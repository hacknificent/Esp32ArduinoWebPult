//createPult / generatePultLayout / init
//setPultLayout
//getPultLayout
//setCommands
//chooseCommand
//doCommand
#include <IRremote.h>
IRsend irsend;

class nifPult {
  private:
    bool isCommandChoosen;
    int command;
    int ledPin;
    unsigned long pultCommands[24];
  public:
    nifPult(int ledPin, unsigned long pultCommands[24]) {
      for (int i = 0; i < 24; i++) {
        this->pultCommands[i] = pultCommands[i]; // can be beather
      }
      
      this->ledPin = ledPin;
      this->isCommandChoosen = false;
      // init();
    }
    
    void init() {
      pinMode(this->ledPin, OUTPUT);      // set the LED pin mode for IR sender
    }
    
    void chooseCommand(int pultCommand) { // set command to do
      this->isCommandChoosen = true;
      this->command = pultCommand;
    }
    
    int getChoosenCommand() {
      return this->command;
    }

    void sendChoosenCommand() { 
      if (this->isCommandChoosen == true) {
        irsend.sendNEC(this->pultCommands[getChoosenCommand()], 32);
        //...
        this->isCommandChoosen = false;
      }
    }

};
