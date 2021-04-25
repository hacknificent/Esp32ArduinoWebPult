////Esp32ArduinoWebPult
//
//#include <WiFi.h>
//#include <IRremote.h>
//#include <LiquidCrystal_I2C.h>
//#include <Wire.h>
//
//const int buttonPin = 2;
//
//const int WIFI_CREDS_NUM = 2;
//const char* WIFI_CREDS[WIFI_CREDS_NUM][2] = {
//        {"TP-LINK-420", "kvartira69wifi"},
//        {"CHISoftware_2G", "Lucky2020"}
//};
//
//const String WEB_PAGE_LAYOUT = "<html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0\"><style>ul.pult {display: flex;flex-wrap: wrap;margin: 0 auto;border: 1px solid #000;border-radius: 30px;width: 90%;padding: 2.5%;}ul.pult li {display: block;flex-basis: 25%;padding-top: 25%;box-sizing: border-box;position: relative;}ul.pult li.disabled::after {content: '';display: block;background: #000;opacity: .6;position: absolute;top: 5%;right: 5%;bottom: 5%;left: 5%;border-radius: 100%;}ul.pult li a {border: 1px solid #000;border-radius: 100%;position: absolute;top: 5%;right: 5%;bottom: 5%;left: 5%;background: #eee;color: #000;text-decoration: none;font-size: 7vw;line-height: 1;display: flex;justify-content: center;align-items: center;}</style></head><body><ul class=\"pult\"><li><a href=\"q\">+</a></li><li><a href=\"w\">-</a></li><li><a href=\"e\">off</a></li><li><a href=\"r\">on</a></li><li><a href=\"t\" style=\"    background-color: #fb1d1d;\"></a></li><li><a href=\"y\" style=\"    background-color: #1fff1f;\"></a></li><li><a href=\"u\" style=\"    background-color: #3838ff;\"></a></li><li><a href=\"i\"></a></li><li><a href=\"o\"></a></li><li><a href=\"p\"></a></li><li><a href=\"a\"></a></li><li><a href=\"s\"></a></li><li><a href=\"d\"></a></li><li><a href=\"ff\"></a></li><li><a href=\"g\"></a></li><li><a href=\"h\"></a></li><li><a href=\"g\"></a></li><li><a href=\"h\"></a></li><li><a href=\"j\"></a></li><li><a href=\"k\"></a></li><li><a href=\"l\"></a></li><li><a href=\"z\"></a></li><li><a href=\"x\"></a></li><li><a href=\"c\"></a></li></ul></body></html>";
//
//LiquidCrystal_I2C lcd(0x27, 16, 2);
//WiFiServer server(80);
//IRsend irsend;
//
//
//
//void setup()
//{
//
//
//    Serial.begin(115200);
//    pinMode(15, OUTPUT);      // set the LED pin mode
//    pinMode(buttonPin, INPUT);// set the touch button pin
//
//    Serial.println();
//    Serial.println();
//    Serial.print("Connecting to ");
//    lcd.init();                      // initialize the lcd
//    lcd.backlight();
//    lcd.setCursor(0, 0);
//    lcd.print("Connecting to :");
//    delay(500);
//
//    int counter = 0;
//    while (counter < WIFI_CREDS_NUM) { // WIFI_CREDS[0][0]
//        lcd.setCursor(0, 1);
//        lcd.print(WIFI_CREDS[counter][0]); // Print connected wifi name
//        delay(200);
//        WiFi.begin(WIFI_CREDS[counter][0], WIFI_CREDS[counter][1]); //Creates wifi connection
//        delay(800);
//
//        if (WiFi.status() == WL_CONNECTED) {
//            lcd.clear();
//            lcd.setCursor(0, 0);
//            lcd.print("Connected to :");
//            delay(1000); //lcd.setCursor(0,2);
//            counter = WIFI_CREDS_NUM;
//        }
//        counter++;
//    }
//    // WiFi.begin(ssid, password);
//
//    while (WiFi.status() != WL_CONNECTED) {
//        lcd.clear();
//        lcd.setCursor(0, 0);
//        lcd.print("WiFi status ERROR");
//        lcd.setCursor(0, 1);
//        lcd.print(WiFi.status());
//        Serial.print(".");
//        delay(500);
//    }
//
//
//
//    Serial.println("");
//    Serial.println("WiFi connected.");
//    Serial.println("IP address: ");
//    Serial.println(WiFi.localIP());
//
//    server.begin();
//
//}
//
//
//char* nificentPages[7][3] = {
//        {"Home", "header", "footer"}, // WiFi.localIP() // if we dont have sub pages = do action
//        {"WiFi", "info", "Test"},
//        {"IRactions", "", ""},
//        {"LTC Client Msg", "", ""},
//        {"Settings", "", ""},
//        {"Power Options", "", ""},
//        {"Debug", "", ""} // vars / conections
//};
//String IRactionsPosts[4] = {"on", "off", "plus", "minus"};
//
//
//
//
//
//
//String lastClientMessage = "None"; // Store message from web client
//
//String nificentOC;
//
//byte nificentOCstatement = 0; // Store current  statement
//char* nificentOCstatements[3] = {
//        "off", "active", "sleep"
//};
//
//byte currentTouchBtnAction = 0; // Store current action
//char* getTouchBtnActionName[6] = { // CHANGE TO getTouchBtnActionName
//        "waiting", "action", "long-action", "double-tap"
//};
//
////char* nificentOCtriggers[6] = {
////"wait", "action", "esc", "prev", "next", "enter"
////};
//
//
//byte currentPage = 0; // Current Page ID
//char* nificentPageHeaders[6] = {
//        "Home", // Page Title
//        "IRactions",
//        "LTC Client Msg",
//        "Settings",
//        "Power Options",
//        "Debug" // vars / conections
//};
//char* nificentPageFooter =  "<<<   ====   >>>";
//
//char* nifPagesData[2][3] = {
//        {"Home", "Wifi-SSID", "192.168.x.x"},
//        {}
//};
//
/////////
//char* PagesActions[6][2] = {
//        {}, //
//        {}, //
//        // {},{lastClientMessage, }, // Last Message From The Client
//        {}, //
//        {"on", "off"}, // Power Options
//        {}, //
//
//};
//
////page_id, ++action_id
//char* nificentPageCustomActions[6][5] = {
//        { // Home
//                "", //action
//                "", // long-action
//                "", // double-tap
//                // Back Light Time?
//        },
//        { //IRactions
//                "", //action
//                "" // long-action
//        },
//        { //    "LTC Client Msg",
//        },
//        { //  "Settings",
//        },
//        { //Power Options
//                "", //action
//                "" // long-action
//        },
//
//};
//
//// Page Slider
////...
//
//
//// int myDisplayBackLightCounter = 0; //   myDisplayBackLightCounter++;
//
//String myDisplayState[2][2] = {
//        { // Current state
//                "0123456789123456",
//                "0123456789123456"
//        },
//        { // New state
//                "0123456789012345",
//                "0123456789123456"
//        }
//};
//// String myText = "1";
//// if(myText.length()){}
//
//
//int touchButtonCounter = 0; // increes when touch button triggered
//int lastTouchWas = 0; // Store information about how long button was not touched
//
//bool myDisplayChanged = 0;
////int  myDisplayBacklightTimer = 0;
//
//void loop() {
//
//    // char* myJustifyContentArgs[5] = {{"start"},{"center"},{"end"},{"spase-between"},{"spase-around"}};
//    // int myDisplayJustifyContent[2] = {1},{3}; // args(header/footer)
//
//    //Touch Button
//    while (digitalRead(buttonPin)) { // while touch button is triggered
//
//        if (lastTouchWas >= 1 && lastTouchWas <= 100) { // check if button was touchd resentli
//            currentTouchBtnAction = 3; // "double-tap"
//        }
//
//        // touchButtonCounter++; // then longer we hold, then hier the number
//        if (touchButtonCounter++ >= 1) {
//            nificentOC = "action";
//            currentTouchBtnAction = 1; // "action"
//            if (touchButtonCounter >= 10) {
//                nificentOC = "longaction";
//                currentTouchBtnAction = 2; // "longaction"
//                //...
//            }
//        }
//        delay(20);
//    }
//
//    if (touchButtonCounter >= 1) { // if Button was touched
//        lastTouchWas = 0; // Reset Last Touch Counter
//        touchButtonCounter = 0; //Reset Touch Button Counter
//    } else {
//        lastTouchWas++; // Count whait time
//    }
//
//    // at the end?
//    if ( lastTouchWas >= 1000 ) {
//        lcd.noBacklight(); // turn off backlight
//    } else if ( lastTouchWas >= 5000 ) {
//        nificentOCstatement = 2;
//        //Sleep time
//    }
//
//    //if(nificentOCstatement == 2){//Is sleep mode
//    //      lcd.noBacklight();
//    // nificentOCstatement
//    //
//    //  ..}
//    //  if (myDisplayBacklightTimer >= 1) {
//    //    myDisplayBacklightTimer++;
//    //    if (myDisplayBacklightTimer >= 200) {
//    //      lcd.noBacklight(); // turn off backlight
//    //    }
//    //  }
//
//    if (nificentOCstatement == 2 && currentTouchBtnAction != 0) {
//        nificentOCstatement = 1; //NificentOC statement is "active" now//
//        lcd.backlight(); // turn on backlight.
//        delay(100);
//    }
//
//    // Enable Backlight if was currentTouchBtnAction
//    if (currentTouchBtnAction != 0) { // if current action is not "wait"
//
//        if (nificentOCstatement != 1) { // if Nificent OC statement is not "active"
//            nificentOCstatement = 1; //NificentOC statement is "active" now//
//            lcd.backlight(); // turn on backlight.
//            delay(100);
//        }
//
//        if (currentTouchBtnAction == 1) { // if touchBtn action is "action"
//            lcd.setCursor(14, 0);
//            lcd.print("> ");
//            delay(150);
//            lcd.setCursor(14, 0);
//            lcd.print(" >");
//            delay(150);
//            currentPage++;
//        } else if (currentTouchBtnAction == 2) { // if touchBtn action is "long-action"
//            int test = 0;
//            while (test < 16) {
//                lcd.setCursor(test++, 1);
//                lcd.print(">");
//                delay(100);
//                currentPage = 0; // Go Home
//            }
//        } else if (currentTouchBtnAction == 3) { // if touchBtn action is "double-tap"
//            currentPage--;
//        } // else if (currentTouchBtnAction == ) { // if touchBtn action is ""
//
//        currentTouchBtnAction = 0; // RESET Nificent OC action to "waiting"
//    }
//
//
//    if (nificentOCstatement == 1) { // If NificentOC is active
//        int paddingNum = 0;
//
//        if ( currentPage == 0 ) { // If Current Page is home
//            //Update Display
//            myDisplayState[1][0] = "WiFi"; //set display header line
//            myDisplayState[1][1] = "HelloWorld";         //set display footer line
//        } else {
//
//            myDisplayState[1][0] = nificentPageHeaders[currentPage]; //set display header line
//            myDisplayState[1][1] = nificentPageFooter; //set display footer line
//        }
//
//        // Display update logic
//        if (myDisplayState[0][0] != myDisplayState[1][0]) { // if display FIRST line current state != New state
//            //lcd.backlight(); // turn on backlight.
//            int strLen = myDisplayState[1][0].length();
//            if (strLen != 16) {
//                int myTrimRange = 16 - strLen;
//                if (myTrimRange >= 2) {
//                    paddingNum = myTrimRange / 2; // 3
//                    // while(--paddingNum >= 0){}
//                }
//
//            }
//            if (myDisplayState[0][0] != myDisplayState[1][0]) {
//                myDisplayState[0][0] = myDisplayState[1][0]; //Current state = new state
//                myDisplayChanged = 1; // Declare that the display is changed
//
//            }
//
//        }
//
//        if (myDisplayState[0][1] != myDisplayState[1][1]) { // if display SECOND line current state != New state
//            myDisplayState[0][1] = myDisplayState[1][1]; //Current state = new state
//            myDisplayChanged = 1; // Declare that the display is changed
//
//        }
//
//        //update my display
//        if (myDisplayChanged == 1) { // if myDisplay changed
//            lcd.clear(); //how to clear a line? / mb set as " "?
//            lcd.setCursor(paddingNum, 0);
//            lcd.print(myDisplayState[0][0]); // print first line current state
//            lcd.setCursor(0, 1);
//            lcd.print(myDisplayState[0][1]); // update second line on display
//            delay(10);
//            //myDisplayBacklightTimer = 1;
//        }
//
//
//
//
//    }
//
//
//
//    WiFiClient client = server.available();   // listen for incoming clients
//
//
//
//    if (client) {                             // if you get a client,
//        Serial.println("New Client.");           // print a message out the serial port
//        Serial.print(client);
//
//        lcd.clear();
//        lcd.backlight(); // turn on backlight.
//        lcd.setCursor(0, 0);
//        // print static message
//        lcd.print("New Client.");
//
//
//        String currentLine = "";                // make a String to hold incoming data from the client
//        while (client.connected()) {            // loop while the client's connected
//            if (client.available()) {             // if there's bytes to read from the client,
//                char c = client.read();             // read a byte, then
//                Serial.write(c);                    // print it out the serial monitor
//                if (c == '\n') {                    // if the byte is a newline character
//                    // if the current line is blank, you got two newline characters in a row.
//                    // that's the end of the client HTTP request, so send a response:
//                    if (currentLine.length() == 0) {
//                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
//                        // and a content-type so the client knows what's coming, then a blank line:
//                        client.println("HTTP/1.1 200 OK");
//                        client.println("Content-type:text/html");
//                        client.println();
//                        client.print(WEB_PAGE_LAYOUT);
//                        //            client.print("<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0\">");
//                        //            client.print("<style>ul.pult {display: flex;flex-wrap: wrap;margin: 0 auto;border: 1px solid #000;border-radius: 30px;width: 90%;padding: 2.5%;}ul.pult li {display: block;flex-basis: 25%;padding-top: 25%;box-sizing: border-box;position: relative;}ul.pult li.disabled::after {content: '';display: block;background: #000;opacity: .6;position: absolute;top: 5%;right: 5%;bottom: 5%;left: 5%;border-radius: 100%;}ul.pult li a {border: 1px solid #000;border-radius: 100%;position: absolute;top: 5%;right: 5%;bottom: 5%;left: 5%;background: #eee;color: #000;text-decoration: none;font-size: 10vw;line-height: 1;display: flex;justify-content: center;align-items: center;}</style>");
//                        //            client.print("</head><body>");
//                        //            client.print("<ul class=\"pult\">");
//                        //            client.print("<li><a href=\"pult/q\">+</a></li><li><a href=\"pult/w\">-</a></li><li><a href=\"pult/e\">on</a></li><li><a href=\"pult/r\">off</a></li><li><a href=\"pult/t\"></a></li><li><a href=\"pult/y\"></a></li><li><a href=\"pult/u\"></a></li><li><a href=\"pult/i\"></a></li><li><a href=\"pult/o\"></a></li><li><a href=\"pult/p\"></a></li><li><a href=\"pult/a\"></a></li><li><a href=\"pult/s\"></a></li><li><a href=\"pult/d\"></a></li><li><a href=\"pult/f\"></a></li><li><a href=\"pult/g\"></a></li><li><a href=\"pult/h\"></a></li><li><a href=\"pult/g\"></a></li><li><a href=\"pult/h\"></a></li><li><a href=\"pult/j\"></a></li><li><a href=\"pult/k\"></a></li><li><a href=\"pult/l\"></a></li><li><a href=\"pult/z\"></a></li><li><a href=\"pult/x\"></a></li><li><a href=\"pult/c\"></a></li>");
//                        //            client.print("</ul>");
//                        //            client.print("</body>");
//                        // The HTTP response ends with another blank line:
//                        client.println();
//                        // break out of the while loop:
//                        break;
//                    } else {    // if you got a newline, then clear currentLine:
//                        currentLine = "";
//                    }
//                } else if (c != '\r') {  // if you got anything else but a carriage return character,
//                    currentLine += c;      // add it to the end of the currentLine
//                }
//
//
//                unsigned long tData = 0xFFE01F;
//                // Check to see if the client request was "GET /H" or "GET /L":
//                if (currentLine.endsWith("GET /q")) {
//                    tData = 0xFF00FF;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /w")) {
//                    tData = 0xFF40BF;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /e")) {
//                    tData = 0xFF609F;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /r")) {
//                    tData = 0xFFE01F;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /t")) {
//                    tData = 0xFF10EF;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /y")) {
//                    tData = 0xFF906F;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /u")) {
//                    tData = 0xFF50AF;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /i")) {
//                    tData = 0xFFC03F;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /o")) {
//                    tData = 0xFF30CF;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /p")) {
//                    tData = 0xFFB04F;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /a")) {
//                    tData = 0xFF708F;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /s")) {
//                    tData = 0xFFF00F;
//                    delay(40);
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /d")) {
//                    tData = 0xFF08F7;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /ff")) {
//                    tData = 0xFF8877;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /g")) {
//                    tData = 0xFF48B7;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /h")) {
//                    tData = 0xFFC837;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /j")) {
//                    tData = 0xFF28D7;
//                    delay(40);
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /k")) {
//                    tData = 0xFFA857;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /l")) {
//                    tData = 0xFFE817;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /z")) {
//                    tData = 0xFF6897;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /x")) {
//                    tData = 0xFF18E7;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /c")) {
//                    tData = 0xFF9867;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /v")) {
//                    tData = 0xFF58A7;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//                } else if (currentLine.endsWith("GET /?test=1")) {
//                    tData = 0xFF58A7;
//                    irsend.sendNEC(tData, 32);
//                    delay(40);
//
//                    int myDelay = 1500;
//
//                    int var = 0;
//                    while (var < 30) {
//                        // do something repetitive 200 times
//                        tData = 0xFFC03F; // white
//                        irsend.sendNEC(tData, 32);
//                        delay(myDelay);
//                        tData = 0xFF10EF; // red
//                        irsend.sendNEC(tData, 32);
//                        delay(myDelay);
//                        tData = 0xFF906F; // green
//                        irsend.sendNEC(tData, 32);
//                        delay(myDelay);
//                        tData = 0xFF50AF; // blue
//                        irsend.sendNEC(tData, 32);
//                        delay(myDelay);
//                        var++;
//                        if (myDelay > 100) {
//                            myDelay = myDelay - 100;
//                        }
//                    }
//
//                    myDelay = 500;
//                    tData = 0xFF906F; // green
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF50AF; // blue
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF906F; // green
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    myDelay = 120;
//                    tData = 0xFF10EF; // red
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF906F; // green
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF10EF; // red
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF906F; // green
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF10EF; // red
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    myDelay = 500;
//                    tData = 0xFF906F; // green
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF10EF; // red
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF906F; // green
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF10EF; // red
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF906F; // green
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF50AF; // blue
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF50AF; // blue
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF50AF; // blue
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                    tData = 0xFF10EF; // red
//                    irsend.sendNEC(tData, 32);
//                    delay(myDelay);
//                }
//
//                
//            }
//        }
//        // close the connection:
//        client.stop();
//        Serial.println("Client Disconnected.");
//        lcd.clear();
//        lcd.backlight(); // turn on backlight.
//        lcd.setCursor(0, 0);
//        lcd.print("Client Disconnected.");
//        delay(500);
//        lcd.noBacklight(); // turn off backlight
//        delay(500);
//        lcd.backlight(); // turn on backlight.
//        delay(500);
//    }
//    delay(100);
//}
