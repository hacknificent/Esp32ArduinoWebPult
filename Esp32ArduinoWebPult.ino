// Esp32 Arduino WebPult for RGB Led lamp/strips
#include <WiFi.h>
#include <IRremote.h>
#include <Wire.h>
#include "config.h"
#include "nifButton.h"
#include "nifDisplay.h"

WiFiServer server(80);
IRsend irsend;
nifButton tButton(2);
nifDisplay lcdDisplay(200);

unsigned long tData = 0xFFE01F;
String pultForm;

void setup() {

  Serial.begin(115200);
  if (Serial.available()) {
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
  }

  pinMode(15, OUTPUT);      // set the LED pin mode for IR sender
  lcdDisplay.init();
  tButton.init();

  WiFi.begin(ssid, password);

  if (Serial.available()) {
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
  }
  delay(300);

  while (WiFi.status() != WL_CONNECTED) {
    delay(128);
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print("WiFi Alert");
  }


  if (Serial.available()) {
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  lcdDisplay.setHeader("Test Header");
  delay(1000);

  lcd.setCursor(0, 0);
  // print static message
  lcd.print(WiFi.localIP());
  delay(80);

  server.begin();

  tData = 0xFF609F; //off
  irsend.sendNEC(tData, 32); // The function sendNEC(data, nbits) is deprecated and may not work as expected! Use sendNECRaw(data, NumberOfRepeats) or better sendNEC(Address, Command, NumberOfRepeats).
  delay(40);

  pultForm += "<form action=\"pult\" target=\"_self\" method=\"get\" >";
  pultForm += "<ul class=\"pult\">";
  //Print Pult Buttons
  for (int i = 0; i < 24; i++) {
    pultForm += "<li class=\"pultButton\"";
    if (pultButtons[i][1] && pultButtons[i][1] != "") {
      pultForm += "style = \"color: #" + pultButtons[i][1] + ";\""; // Print button color
    }
    pultForm += ">";

    if (pultButtons[i][0] && pultButtons[i][0] != "") {

      pultForm += "<span>";
      pultForm += pultButtons[i][0];// Print button Label
      pultForm += "</span>";
    }

    pultForm += "<input type=\"submit\" name=\"pBtn\" value=\"" + String(i) + "c\">";


    pultForm += "</li>";
  }
  pultForm += "</ul></form>";

}




//LampVars
bool nifLampPowerStatement = false;
int nifLampMode = 0; // 0: White; 1: Smooth; (Switches on long-action)

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  lcdDisplay.updateDisplayBacklight();

  switch (tButton.getState()) {
    case 1: // "action"
      //displayBacklightTimer = displayBacklightTime / 2;
      lcdDisplay.startBacklightTimer();
      break;
    case 2:// "longAction"
      // Change Light Mode (White/Smooth)
      if (nifLampMode == 0) {
        tData = 0xFFC03F; // White
        nifLampMode = 1;
      } else if (nifLampMode == 1) {
        tData = 0xFFE817; //Smooth
        nifLampMode = 0;
      }
      irsend.sendNEC(tData, 32);
      delay(40);
      tButton.resetState();
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
      //currentTouchBtnAction = 0;
      //lastTouchActionWas = 0;
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
            client.print(pultForm);

            client.print("<form action=\"pultTest\" target=\"_self\" method=\"post\" ><ul class=\"pult\">");

            client.print("<li class=\"pultButton\"");

            client.print("style = \"flex-basis: 100%;\">"); // Print button color

            client.print("<span>Pult Test</span>"); // Print button Label
            client.print("<input type=\"submit\" name=\"btn\" value=\"Submit\">");
            client.print("</li>");


            client.print("</ul></form>");



            //client.print("<script>alert(\"Test\")</script></body>");
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
          //displayBacklightTimer = displayBacklightTime / 2;
          lcdDisplay.startBacklightTimer();

        }






        if (currentLine.endsWith("GET /pult?pBtn=")) {
          getCommand = true; //
        }

        if (getCommand == true) {
          for (int i = 0; i < 24; i++) {
            if (currentLine.endsWith("pBtn=" + String(i) + 'c')) {
              //lcd.clear();
              lcd.setCursor(0, 1);
              lcd.print("Command: ");
              lcd.print(i);
              delay(80);
              //lcd.backlight(); // turn on backlight.
              //displayBacklightTimer = displayBacklightTime / 2;
              lcdDisplay.startBacklightTimer();

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
