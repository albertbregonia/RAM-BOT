# RAM-BOT
This repository serves as an archive for Team *Ram, Egg and Cheese*'s Stony Brook Robotics Fall 2021 Internal Competition Code.

## Overview
*Ram-Jam* (as coined by Sila) the RAM-BOT is a remotely controlled castle destroyer robot. His system works by using a NodeMCU ESP8266 microcontroller running a web server that will process HTTP requests as incoming commands and relay them to an Arduino Nano Every over UART. The web server serves a frontend that can be used by any JavaScript capable device to control *Ram-Jam*. There is a limit of two connections on *Ram-Jam*'s access point.

The team consists of:
- Albert Bregonia
- Adam Nebylitsa
- Chris Moriarty
- Sila Atalar

# Repository Structure
- The [`RAM-BOT`](https://github.com/albertbregonia/ram-bot/tree/main/RAM-BOT) directory consists of code put on the Arduino Nano Every
- The [`RAM-WiFi`](https://github.com/albertbregonia/ram-bot/tree/main/RAM-WiFi) directory consists of code put on the NodeMCU ESP8266
