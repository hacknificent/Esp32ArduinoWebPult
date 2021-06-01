// Esp32 Arduino WebPult for RGB Led lamp/strips
#include <WiFi.h>

#include <Wire.h>
#include "config.h"
#include "nifButton.h"
#include "nifDisplay.h"
#include "nifPult.h"

WiFiServer server(80);
nifPult pult(15, pultCommands);
nifButton tButton(2);
nifDisplay lcdDisplay(200);


String nifGetRequest( String clientLine) { //clear request / sanitize
  String clientRequest = clientLine;
  clientRequest.replace(" HTTP/1.1", "/");
  clientRequest.replace("GET /", "/");
  clientRequest.replace("//", "/");
  return clientRequest;
}


unsigned long tData = 0xFFE01F;
String pultForm;

void setup() {

  Serial.begin(115200);
  if (Serial.available()) {
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
  }

  // set the LED pin mode for IR sender
  pult.init();
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

  //  pultForm += "<form action=\"pult\" target=\"_self\" method=\"get\" >";
  pultForm += "<ul class=\"pult\">";
  //Print Pult Buttons
  for (int i = 0; i < 24; i++) {
    pultForm += "<li>";

    pultForm += "<button class=\"pBtn\"btnData = \"" + String(i) + "\" ";
    if (pultButtons[i][1] && pultButtons[i][1] != "") {
      pultForm += "style = \"background-color: #" + pultButtons[i][1] + ";\""; // Print button color
    }
    pultForm += ">";

    if (pultButtons[i][0] && pultButtons[i][0] != "") {


      pultForm += pultButtons[i][0];// Print button Label

    }
    pultForm += "</button>";

    //pultForm += "<input type=\"submit\" name=\"pBtn\" value=\"" + String(i) + "c\">";
    //<button type="button">Click Me!</button>

    pultForm += "</li>";
  }
  pultForm += "</ul>";// </form>";

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
    String getRequest = "";
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {


          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }


        //Check if currentLine is GET client request
        if (currentLine.endsWith("HTTP/1.1")) {
          if (currentLine.startsWith("GET /")) {
            getRequest = nifGetRequest(currentLine); //return
          }
        }

        if (getRequest != "") {
          if (getRequest.startsWith("/")) {

            if (getRequest == "/pult/") {

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
              client.print("<iframe id=\"progressive-iframe\" src=\"/command/3/\" frameborder=\"0\" seamless></iframe>");
              //client.print("<script type="text/javascript">alert(\"Test\")</script></body>"); // !!! text/javascript
              client.print("<script>");
              //client.print(script);
              client.print("</script>");
              client.print("</body>");
              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;

            } else if (getRequest.startsWith("/command/")) {
              for (int i = 0; i < 24; i++) {
                if (getRequest.endsWith('/' + String(i) + '/' )) {
                  pult.chooseCommand(i); // !!!
                  //break;
                }
              }
              client.println("HTTP/1.1 418 I'm a teapot");
              client.println(); // !!!
              break;
            } else if (getRequest == "/destruct/") {
              String style = "body {margin: 0;padding: 0;width: 100vw;height: 100vh;display: flex;justify-content: center;align-items: center;overflow: hidden;background-color: #151515;}.base {background: #cacaca;width: 20vmin;border-radius: 27vmin;box-shadow: 0 6vmin 0.15vmin 0vmin #777, 0 4vmin 0.15vmin 0vmin #777, 0 2vmin 0.15vmin 0vmin #777;padding: 0vmin 2vmin 2vmin 2vmin;z-index: 1;transform: rotateX(60deg) rotateZ(0deg);margin-top: -4.5vmin;height: 22vmin;}button#activate {background: #d60505;border: 0;width: 20vmin;height: 19vmin;border-radius: 100%;position: relative;cursor: pointer;outline: none;z-index: 2;box-shadow: 0 4vmin 0.15vmin 0vmin #af0000, 0 2vmin 0.15vmin 0vmin #af0000;top: -2.5vmin;border: 0.5vmin solid #af0000a1;transition: all 0.25s ease 0s;}button#activate:hover {box-shadow: 0 3vmin 0.15vmin 0vmin #af0000, 0 1vmin 0.15vmin 0vmin #af0000;top: -1.5vmin;transition: all 0.5s ease 0s;}button#activate:active, button#activate.pushed {box-shadow: 0 1vmin 0.15vmin 0vmin #af0000, 0 1vmin 0.15vmin 0vmin #af0000;top: 0.5vmin;transition: all 0.25s ease 0s;}button#activate.pushed {box-shadow: 0 0 20px 10px #ff3c3c, 0 0 100px 50px #ff2828;background: #ff0000;border-bottom: 3px solid #00000020;}.box {transform: rotateX(-35deg) rotateY(45deg) rotateZ(0deg) rotate3d(1, 0, 0, 90deg);transform-origin: center top;transform-style: preserve-3d;width: 45vmin;position: absolute;z-index: 5;margin-top: 27vmin;transition: transform 1s ease 0s;cursor: pointer;height: 45vmin;margin-left: -32vmin;}.box.opened {transform: rotateX(-35deg) rotateY(45deg) rotateZ(0deg) rotate3d(1, 0, 0, 180deg);}.box div {position: absolute;width: 45vmin;height: 45vmin;background: #00bcd47d;opacity: 0.5;border: 3px solid #00a4b9;border-radius: 3px;box-sizing: border-box;box-shadow: 0 0 3px 0 #00bcd48a;}.box > div:nth-child(1) {opacity: 0;}.box > div:nth-child(2) {transform: rotateX(90deg) translate3d(0px, 5vmin, 5vmin);height: 10vmin;}.box > div:nth-child(3) {transform: rotateX(0deg) translate3d(0, 0, 10vmin);}.box > div:nth-child(4) {transform: rotateX(270deg) translate3d(0px, -5vmin, 40vmin);height: 10vmin;}.box > div:nth-child(5) {transform: rotateY(90deg) translate3d(-5vmin, 0, 40vmin);width: 10vmin;}.box > div:nth-child(6) {transform: rotateY(-90deg) translate3d(5vmin, 0vmin, 5vmin);width: 10vmin;}.grid {background:repeating-linear-gradient(150deg, rgba(255,255,255,0) 0, rgba(255,255,255,0) 49px, rgb(255 255 255 / 10%) 50px ,rgb(0 0 0 / 30%) 51px , rgba(255,255,255,0) 55px ), repeating-linear-gradient(30deg, rgba(255,255,255,0) 0, rgba(255,255,255,0) 49px, rgb(255 255 255 / 10%) 50px ,rgb(0 0 0 / 30%) 51px , rgba(255,255,255,0) 55px );position: fixed;width: 200vw;height: 150vh;}.warning {position: absolute;z-index: 0;width: 45vmin;height: 45vmin;background: repeating-linear-gradient(-45deg, black, black 3vmin, yellow 3vmin, yellow 6vmin);transform: rotateX(-35deg) rotateY(45deg) rotateZ(0deg) rotate3d(1, 0, 0, 90deg);box-shadow: 0 0 0 3vmin #af0000;}.warning:before {content: \"\";width: 80%;height: 80%;background: linear-gradient(45deg, #000000 0%, #414141 74%);float: left;margin-top: 10%;margin-left: 10%;border: 1vmin solid yellow;border-radius: 1vmin;box-sizing: border-box;}.warning:after {content: \"WARNING:\\2009 DANGER\";color: white;transform: rotate(90deg);float: left;background: #af0000;position: absolute;bottom: 18.5vmin;left: -35vmin;font-size: 5vmin;font-family: Arial, Helvetica, serif;width: 49vmin;text-align: center;padding: 1vmin;text-shadow: 0 0 1px #000, 0 0 1px #000, 0 0 1px #000;}.hinges {position: absolute;z-index: 3;transform: rotateX(-35deg) rotateY(45deg) rotateZ(0deg) rotate3d(1, 0, 0, 90deg);}.hinges:before, .hinges:after {content: \"\";background: #2b2b2b;width: 5vmin;height: 1.5vmin;position: absolute;margin-top: -24.5vmin;z-index: 5;border: 2px solid #00000010;border-radius: 5px 5px 0 0;box-sizing: border-box;margin-left: -16.25vmin;}.hinges:after {margin-left: 13.75vmin;margin-top: -24.5vmin;}.box > span:before, .box > span:after {content: \"\";width: 5vmin;height: 1.5vmin;background: #103e4480;position: absolute;margin-left: 6vmin;border-radius: 0 0 5px 5px;}.box > span:after  {margin-left: 36vmin;}.box > span {transform: rotateX(89deg) translate(0.3vmin, 0.3vmin);position: absolute;}.text {position: absolute;margin-top: 55vmin;color: white;font-family: Arial, Helvetica, serif;font-size: 5vmin;text-shadow: 0 0 1px #000, 0 0 1px #000, 0 0 1px #000;perspective-origin: left;background: #af0000;padding: 1vmin;transform: rotateX(-35deg) rotateY(45deg) rotateZ(0deg) rotate3d(1, 0, 0, 90deg) translate(33.5vmin, -2vmin);text-align: center;width: 49vmin;}div#panel:before {content: \"WARNING\";top: 3vmin;position: relative;font-size: 10vmin;width: 100vw;left: 0;z-index: 6;text-shadow: 0 0 1px #fff, 0 0 3px #fff;border-bottom: 1vmin dotted #fff;}#panel {position: absolute;background: #ff0000d0;color: #ffffff;font-family: Arial, Helvetica, serif;width: 90vmin;box-sizing: border-box;font-size: 3.25vmin;padding: 1vmin 2vmin;height: 60vmin;box-shadow: 0 0 0 100vmin #ff000060, 0 0 0 5vmin #ff000060;z-index: 5;display: none;text-align: center;text-shadow: 0 0 1px #fff, 0 0 3px #fff, 0 0 5px #fff;animation: warning-ligth 1s 0s infinite;}#panel.show {display: block !important;}#msg {margin-top: 5vmin;text-shadow: 0 0 2px #fff;}#time {font-size: 10vmin;background: #00000080;max-width: 35vmin;margin: 6vmin auto 5vmin !important;position: relative;border-radius: 0.25vmin;text-shadow: 0 0 3px #000, 0 0 2px #000, 0 0 3px #000, 0 0 4px #000, 0 0 5px #000;padding: 1vmin 0;}#time:before {content: \"00:0\";:}#abort {background: #ffffffb8;color: #d30303;cursor: pointer;padding: 1vmin 2.75vmin;font-size: 6vmin;border-radius: 0.25vmin;font-weight: bold;animation: highlight 1s 0s infinite;}#abort:hover {background: #ffffff;box-shadow: 0 0 15px 5px #fff;}@keyframes highlight {50% { box-shadow: 0 0 15px 5px #fff;}}div#turn-off {position: fixed;background: #ffffff80;left: 0;width: 100vw;height: 0vh;z-index: 7;}div#turn-off:before, div#turn-off:after {content: \"\";position: fixed;left: 0;top: 0;height: 0vh;background: #000;width: 100vw;transition: height 0.5s ease 0s;}div#turn-off:after {top: inherit;bottom: 0;}div#turn-off.close {height: 100vh;}div#turn-off.close:before, div#turn-off.close:after {transition: height 0.1s ease 0.1s;height: 49.75vh;}#time.crono {background: #ffffffba;transition: background 0.5s ease 0s;color: #ff0000;text-shadow: 0 0 1px #ffffff, 0 0 2px #ffffff, 0 0 2px #ffffff;}#detonate {display: none;color: #fff;z-index: 5;font-size: 8vmin;font-family: Arial, Helvetica, serif;text-shadow: 0 0 1px #fff, 0 0 2px #fff, 0 0 3px #fff;}#detonate.show {display: block;animation: blink 0.25s 0s infinite;}#abort.hide {display: none;}@keyframes blink {50% { opacity: 0;}}#closing {width: 100vw;height: 100vh;left: 0;position: absolute;}div#closing:before, div#closing:after {content: \"\";width: 50vw;height: 1.5vh;left: -50vw;top: 49vh;position: absolute;background: #000000;z-index: 7;transition: left 0.2s ease 0s;}div#closing:after {right: -50vw;transition: right 0.2s ease 0s;left: initial;}div#closing.close:before {left: 0;transition: left 0.2s ease 0.2s;}div#closing.close:after {right: 0;transition: right 0.2s ease 0.2s;}#restart {position: absolute;z-index: 8;display: none;}#reload {position: absolute;z-index: 8;width: 10vmin;height: 10vmin;border-radius: 100%;border: 0;margin-top: -5vmin;margin-left: -2.5vmin;opacity: 0;cursor: pointer;transform: rotate(0deg);transition: transform 0.5s ease 0s;outline: none;}#reload:hover {background: #ef0000;transform: rotate(360deg);transition: transform 0.5s ease 0s;}#restart.show {display: block;}#restart.show #reload {animation: refresh 3.5s 0s 1;opacity:1;}@keyframes refresh {0% { opacity: 0; }50% { opacity: 0; }100% { opacity: 1; }}button#reload:before {content: \"\";width: 6vmin;height: 6vmin;position: absolute;left: 2vmin;top: 2vmin;border-radius: 100%;border: 1vmin solid #000;box-sizing: border-box;border-bottom-color: transparent;}button#reload:after {content: \"\";border: 1.25vmin solid transparent;border-top: 2vmin solid black;position: absolute;transform: rotate(40deg) translate(0.5vmin, 1.25vmin);}@keyframes warning-ligth {0% { box-shadow: 0 0 0 100vmin #ff000060, 0 0 0 5vmin #ff000060; }50% { box-shadow: 0 0 0 100vmin #ff000020, 0 0 0 5vmin #ff000020; }}#mute {position: absolute;bottom: 1vmin;right: 1vmin;background: #8bc34a80;width: 6vmin;height: 6vmin;cursor: pointer;border: 0.5vmin solid #151515;}#mute.muted {background: #ff000080;}#mute:before {content: \"\";border: 0.75vmin solid transparent;height: 2vmin;border-right: 2vmin solid #151515;position: absolute;border-left-width: 0;top: 1.25vmin;right: 1.25vmin;}#mute:after {content: \"\";border: 0vmin solid transparent;height: 2vmin;border-right: 1.5vmin solid #151515;position: absolute;top: 2vmin;right: 3.5vmin;}";
              String script = "var theCount;var alarm = document.getElementById(\"alarm\");var panel = document.getElementById(\"panel\");var turnOff = document.getElementById(\"turn-off\");var turnOffHor = document.getElementById(\"closing\");var detonate = document.getElementById(\"detonate\");alarm.volume = 0.99;var time = document.getElementById(\"time\");function showCountDown() {time.innerText = time.innerText - 1;if (time.innerText == 0) {clearInterval(theCount);time.classList.add(\"crono\");abort.classList.add(\"hide\");detonate.classList.add(\"show\");setTimeout(function () {turnOff.classList.add(\"close\");turnOffHor.classList.add(\"close\");reload.classList.add(\"show\");alarm.pause();}, 1500);}}var cover = document.getElementById(\"cover\");cover.addEventListener(\"click\", function () {if (this.className == \"box\") this.classList.add(\"opened\");else this.classList.remove(\"opened\");});var btn = document.getElementById(\"activate\");activate.addEventListener(\"click\", function () {this.classList.add(\"pushed\");alarm.load();alarm.currentTime = 10.1;alarm.play();setTimeout(function () {panel.classList.add(\"show\");theCount = setInterval(showCountDown, 1000);alarm.load();alarm.play();}, 500);});var abort = document.getElementById(\"abort\");abort.addEventListener(\"click\", function () {btn.classList.remove(\"pushed\");panel.classList.remove(\"show\");clearInterval(theCount);time.innerText = 9;alarm.pause();alarm.currentTime = 10;alarm.play();});var reload = document.getElementById(\"restart\");reload.addEventListener(\"click\", function () {panel.classList.remove(\"show\");turnOff.classList.remove(\"close\");turnOffHor.classList.remove(\"close\");abort.classList.remove(\"hide\");detonate.classList.remove(\"show\");cover.classList.remove(\"opened\");btn.classList.remove(\"pushed\");this.classList.remove(\"show\");time.classList.remove(\"crono\");time.innerText = 9;});setTimeout(function () {cover.classList.remove(\"opened\");}, 100);var mute = document.getElementById(\"mute\");mute.addEventListener(\"click\", function () {if (this.className == \"muted\") {alarm.muted = false;this.classList.remove(\"muted\");} else {alarm.muted = true;this.classList.add(\"muted\");}});";
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();
              client.print("<head>");
              client.print("<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0\">");
              client.print("<style>" + style + "</style>");
              client.print("</head><body>");
              //Page Layout
              client.print("<div class=\"grid\"></div><div class=\"warning\"></div><div class=\"base\">    <button id=\"activate\">        <span></span>    </button></div><div class=\"box opened\" id=\"cover\">    <div></div>    <div></div>    <div></div>    <div></div>    <div></div>    <div></div>    <span></span><span></span></div><div class=\"hinges\"></div><div class=\"text\">    SELF-&thinsp;DESTRUCT</div><div id=\"panel\">    <div id=\"msg\">DEVICE SELF-DESTRUCTION </div>    <div id=\"time\">9</div>    <span id=\"abort\">ABORT</span>    <span id=\"detonate\">DETONATE</span></div><div id=\"turn-off\"></div><div id=\"closing\"></div><div id=\"restart\"><button id=\"reload\"></button></div><div id=\"mute\"></div><audio id=\"alarm\">    <source src=\"https://josetxu.com/demos/sounds/self-destruct-alarm-count.mp3\" type=\"audio/mpeg\"></audio>");

              client.print("<script>");
              client.print(script);
              client.print("</script>");
              client.print("</body>");
              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;

            } else if (getRequest == "/settings/") {
              client.println("HTTP/1.1 418 I'm a teapot");
              client.println();
              break;
            } else { /*if (getRequest == "/favicon.ico")*/
              client.println("HTTP/1.1 404 NOT FOUND");
              client.println();
              break;
            }

          }
        }
      }

    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
    // delay(40);

  }
  pult.sendChoosenCommand();
}
