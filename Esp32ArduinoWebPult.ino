// Esp32 Arduino WebPult for RGB Led lamp/strips
#include <WiFi.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "config.h" // Contains project settings

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
  {"off", "000", ""}, // 2
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

LiquidCrystal_I2C lcd(0x27, 16, 2); // See http://playground.arduino.cc/Main/I2cScanner how to test for a I2C device.
WiFiServer server(80);
IRsend irsend;

unsigned long tData = 0xFFE01F;


void setup() {

  Serial.begin(115200);
  if (Serial.available()) {
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
  }


  pinMode(15, OUTPUT);      // set the LED pin mode for IR sender
  pinMode(buttonPin, INPUT);// set the touch button pin

  lcd.init();                      // initialize the lcd
  lcd.backlight();
  delay(80);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  lcd.setCursor(0, 0);
  lcd.print(ssid);
  delay(300);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(128);
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print("WiFi Alert");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  // print static message
  lcd.print(WiFi.localIP());
  // lcd.setCursor(0, 1);
  // lcd.print(ssid);
  delay(80);

  server.begin();

  tData = 0xFF609F;
  irsend.sendNEC(tData, 32); // The function sendNEC(data, nbits) is deprecated and may not work as expected! Use sendNECRaw(data, NumberOfRepeats) or better sendNEC(Address, Command, NumberOfRepeats).
  delay(40);
}




//BtnVars
int currentTouchBtnAction = 0; // Btn Actions: 0 : "whait"; 1 : "action"; 2 : "longAction"; 3: "doubleTap";
int lastTouchActionWas = 0; // Store information about how long button was not touched

//DisplayVars
bool displayBacklight = true;
int displayBacklightTime = 250; // Sets how long backlight will work after touch btn action
int displayBacklightTimer = displayBacklightTime;

//LampVars
bool nifLampPowerStatement = false;
int nifLampMode = 0; // 0: White; 1: Smooth; (Switches on long-action)


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients


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


  //Touch Button
  int touchButtonCounter = 0; // increes when touch button triggered
  while (digitalRead(buttonPin)) { // while touch button is triggered

    if (lastTouchActionWas > 1 && lastTouchActionWas <= 10) { // if button was touched resentli
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
  if (currentTouchBtnAction != 0) { // If btn statemant is not "whait"
    if (++lastTouchActionWas >= 10) {
      currentTouchBtnAction = 0; // Sets btn statemant as "whait"
      lastTouchActionWas = 0;
    }
  }
  switch (currentTouchBtnAction) {
    case 1: // "action"
      displayBacklightTimer = displayBacklightTime;
      break;
    case 2:// "longAction"
      // Change Light Mode
      if (nifLampMode == 0) {
        tData = 0xFFC03F; // White
        nifLampMode = 1;
      } else if (nifLampMode == 1) {
        tData = 0xFFE817; //Smooth
        nifLampMode = 0;
      }
      irsend.sendNEC(tData, 32);
      delay(40);
      currentTouchBtnAction = 0;
      lastTouchActionWas = 0;
      break;
    case 3:// "doubleTap"
      // Light On/Off
      if (nifLampPowerStatement) {
        tData = 0xFF609F;//off
        nifLampPowerStatement = false;
      } else {
        tData = 0xFFE01F;//on
        nifLampPowerStatement = true;
      }
      currentTouchBtnAction = 0;
      lastTouchActionWas = 0;
      irsend.sendNEC(tData, 32);
      delay(40);
      break;

      //    default:
      //      // if nothing else matches, do the default
      //      // default is optional
      //      break;
  }


  bool getCommand = false; //Changes when user ask to do pultCommand
  
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("<head>");
            client.print("<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0\">");
            client.print("<style>" + MAIN_CSS + "</style>");
            client.print("</head><body>");

            client.print("<form action=\"pult\" target=\"_self\" method=\"get\" ><ul class=\"pult\">");
            //Print Pult Buttons
            for (int i = 0; i < 24; i++) {
              client.print("<li class=\"pultButton\"");
              if (pultButtons[i][1] && pultButtons[i][1] != "") {
                client.print("style = \"color: #" + pultButtons[i][1] + ";\""); // Print button color
              }
              client.print(">");
              if (pultButtons[i][0] && pultButtons[i][0] != "") {
                client.print("<span>" + pultButtons[i][0] + "</span>"); // Print button Label
              }
              client.print("<input type=\"submit\" name=\"pBtn\" value=\"" + String(i) + "c\">");
              client.print("</li>");
            }

            client.print("</ul></form>");



            client.print("<form action=\"pultTest\" target=\"_self\" method=\"post\" ><ul class=\"pult\">");

            client.print("<li class=\"pultButton\"");

            client.print("style = \"flex-basis: 100%;\">"); // Print button color

            client.print("<span>Pult Test</span>"); // Print button Label              client.print("<input type=\"submit\" name=\"btn\" value=\"" + String(i) + "c\">");
            client.print("<input type=\"submit\" name=\"btn\" value=\"Submit\">");
            client.print("</li>");


            client.print("</ul></form>");



            client.print("</body>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }


        if (currentLine.endsWith("Submit")) {
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("Sub!!!");
          delay(80);
          //lcd.backlight(); // turn on backlight.
          displayBacklightTimer = displayBacklightTime / 2;

        }






        if (currentLine.endsWith("GET /pult?pBtn=")) {
          getCommand = true; //
        }

        if (getCommand == true) {
          for (int i = 0; i < 24; i++) {
            if (currentLine.endsWith("pBtn=" + String(i) + 'c')) {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Command: ");
              lcd.print(i);
              delay(80);
              //lcd.backlight(); // turn on backlight.
              displayBacklightTimer = displayBacklightTime / 2;

              irsend.sendNEC(pultCommands[i], 32);
              delay(80);
              getCommand = false;
            }
          }
        }



        if (currentLine.endsWith("GET /?test=1")) {
          for (int i = 0; i < 100; i++) {
            tData = 0xFF609F;
            irsend.sendNEC(tData, 32);
            delay(500);
            tData = 0xFFE01F;
            irsend.sendNEC(tData, 32);
            delay(500);
          }
        }

      }

    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
    // delay(40);

  }
}
