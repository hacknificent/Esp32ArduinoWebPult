// class nifWPButton {
//   private:
//     byte pin;
//
//   public:
//     nifWPButton(byte pin) {
//       // Use 'this->' to make the difference between the
//       // 'pin' attribute of the class and the
//       // local variable 'pin' created from the parameter.
//       this->pin = pin;
//       init();
//     }
//     void init() {
//       pinMode(pin, OUTPUT);
//       off();
//     }
//     void on() {
//       delay(20);
//     }
//     void off() {
//       delay(20);
//     }
// };
//
// class nifWPServer {
//
// };
//
// class nifWebPult {
//   private:
//     unsigned long pultCommands[24];
//     String pultButtons[24][3];
//   public:
//     nifWebPult(unsigned long pultCommands[24]) {
//       // Use 'this->' to make the difference between the
//       // 'pin' attribute of the class and the
//       // local variable 'pin' created from the parameter.
//       //this->pin = pin;
//       init();
//     }
//     void init() {
//       //pinMode(pin, OUTPUT);
//       off();
//     }
//     void off() {
//       delay(20);
//     }
//
// };