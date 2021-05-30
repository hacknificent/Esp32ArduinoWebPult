// WiFi Server
const char* ssid     = "Artem_S.o.S.i";
const char* password = "159q2s75";

//CSS Styles
const String MAIN_CSS = "ul.pult {  display: flex;  flex-wrap: wrap;  margin: 0 auto;  border: 1px solid #000;  border-radius: 30px;  width: 90%;  padding: 2.5%;}ul.pult li {  display: block;  flex-basis: 25%;  padding-top: 25%;  box-sizing: border-box;  position: relative;  color: #eee;}ul.pult li.disabled::after {  content: \"\";  display: block;  background: #000;  opacity: 0.6;  position: absolute;  top: 5%;  right: 5%;  bottom: 5%;  left: 5%;  border-radius: 100%;}ul.pult li::before, ul.pult li input[type=submit] {  content: \"\";  display: block;  z-index: -1;  border: 1px solid #000;  border-radius: 100%;  position: absolute;  top: 5%;  right: 5%;  bottom: 5%;  left: 5%;  background: currentColor;  text-decoration: none;  font-size: 7vw;  line-height: 1;}ul.pult li input[type=submit] {  width: 90%;  padding: 0;  color: transparent;  z-index: 2;  font-size: 0;  outline: none !important;}ul.pult li span {  color: #000;  text-align: center;  position: absolute;  top: 50%;  left: 50%;  transform: translate(-50%, -50%);}/*# sourceMappingURL=main.css.map */";

//PultVars
unsigned long pultCommands[24] {
  0xFF00FF, 0xFF40BF, 0xFF609F, 0xFFE01F,
  0xFF10EF, 0xFF906F, 0xFF50AF, 0xFFC03F,
  0xFF30CF, 0xFFB04F, 0xFF708F, 0xFFF00F,
  0xFF08F7, 0xFF8877, 0xFF48B7, 0xFFC837,
  0xFF28D7, 0xFFA857, 0xFF6897, 0xFFE817,
  0xFF18E7, 0xFF9867, 0xFF58A7, 0xFFD827
}; // The function sendNEC(data, nbits) is deprecated and may not work as expected! Use sendNECRaw(data, NumberOfRepeats) or better sendNEC(Address, Command, NumberOfRepeats).
String pultButtons[24][3] = {
  {"+", "", ""}, // 0
  {"-", "", ""}, // 1
  {"off", "505050", ""}, // 2
  {"on", "e80101", ""}, // 3

  {"R", "e80101", ""}, // 4
  {"G", "02cc02", ""}, // 5
  {"B", "403FFD", ""}, // 6
  {"W", "", ""}, // 7

  {"", "ff6000", ""}, // 8
  {"", "46ffc5", ""}, // 9
  {"", "605df7", ""}, // 10
  {"Flash", "", ""}, // 11

  {"", "ffa500", ""}, //12
  {"", "5afff8", ""}, // 13
  {"", "ffc4fd", ""}, // 14
  {"Strobe", "", ""}, // 15

  {"", "ffd125", ""}, // 16
  {"", "4fd3ff", ""}, // 17
  {"", "f5a0ff", ""}, // 18
  {"Smooth", "", ""}, // 19

  {"", "ffff00", ""}, // 20
  {"", "6b9eff", ""}, // 21
  {"", "ff92ce", ""}, // 22
  {"Mode", "", ""}, // 23

};

// Pult Buttons
