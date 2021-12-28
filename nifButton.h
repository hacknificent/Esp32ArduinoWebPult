class nifButton {
  private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

    int currentTouchBtnAction = 0; // Btn Actions: 0 : "whait"; 1 : "action"; 2 : "longAction"; 3: "doubleTap";
    int lastTouchActionWas = 0; // Store information about how long button was not touched
    int touchButtonCounter = 0; // increase when touch button triggered

  public:
    nifButton(byte pin) {
      this->pin = pin;
      init();
    }
    void init() {
      pinMode(pin, INPUT);
      updateState();
    }
    void updateState() {
      if (digitalRead(pin)) {

        if (lastTouchActionWas > 1 && lastTouchActionWas <= 10) { // if button was touched recently
          currentTouchBtnAction = 3; // "doubleTap"
        } else {
          currentTouchBtnAction = 1; // "action"

          if (++touchButtonCounter >= 10) {
            currentTouchBtnAction = 2; // "longAction"
            //...
          }
        }
        delay(20);
      }



      if (currentTouchBtnAction != 0) { // If btn statement is not "whait"
        if (++lastTouchActionWas >= 10) {
          resetState();  // Sets btn statement as "whait"
        }
      }

    }

    void resetState() {
      this->currentTouchBtnAction = 0;
      this->lastTouchActionWas = 0;
    }

    int getState() {
      updateState();
      int btnState = currentTouchBtnAction;
      if (currentTouchBtnAction > 1) {
        resetState();
        this->touchButtonCounter = 0;
      }
      return  btnState;
    }

    bool isPressed() {
      return (getState() != 0);
    }
};
