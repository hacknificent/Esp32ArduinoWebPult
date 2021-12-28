v.0.01

Esp32ArduinoWebPult is my project to control RGB lamps and strips using a browser instead of using the included IR remotes. WebPult allows you to control lighting from any device with a browser and access to your local WiFi network.

Now I do not put my classes / objects / methods / functions and leave everything in the form in which it is similar to the original examples from the libraries from which I started.
Now the device consists of:
- The touch button is responsible for turning on the LCD1602 and for switching the light mode when pressed twice or long;
- LCD1602 display showing the ip address of the local web server;
- Led IR, for data transmission;
- Esp32;
  - A web server with a layout that repeats a real IR remote control that controls an RGBW lamp. When you click on the link in the button, you go to a separate route that triggers an infrared diode that reproduces signals similar to the remote control. This gives us the ability to control an RGBW lamp (or any other device that receives IR signals).

Plans:
- Implementation of OOP, creating your own classes and objects of the button / display / web server to abstract work with them. (V.0.02)
- Realization of Master / Slave connection with arduino to control light bulbs in other rooms. For this I am planning to create a separate repository called Esp32ArduinoWebPultMaster / Esp32ArduinoWebPultSlave. In this case, the slave unit will be based on the arduino nano and HC05 bluetooth module.
