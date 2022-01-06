#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <string.h>

#include "frontend/frontend.h"

#define BAUD_RATE 115200

enum Command {
    MOVE_FORWARD,
    MOVE_BACKWARD,
    TURN_LEFT,
    TURN_RIGHT,
    LEFT_SWEEP,
    RIGHT_SWEEP,
    SWEEP_ROUTINE,
    RAM,
    STOP,
};

ESP8266WebServer server(80); //HTTP webserver object
int command, value;

/**
 * Main function to handle requests from the WebApp frontend.
 * Using the `fetch()` API in JavaScript, requests are made by the frontend
 * and will be handled here. This function works by making a request to `192.168.4.1/?cmd=`
 * with the appended command string (ie: `1-2-3-4-5` of the format `command-args0->args3`)
 * as a query string.
 * 
 * Only the "cmd" parameter string is read, the rest are ignored.
 * For example, for a query string of `/?cmd=1-2-3-4-5&a=b, everything after `-5` will be ignored.
 * Similarly, for a query string of `/?a=b&cmd=1-2-3-4-5&c=d`, `a=b&` and `&c=d` will all be ignored.
 * 
 * If no command is given, this function acts as a `handleRoot()`:
 * Handles a request to access the index of the WebApp.
 * Responds to the request with a status code of OK (200) and the entire
 * frontend string (FRONTEND) that was compiled by `html_to_header.py` and placed in `frontend.h` 
 */
void commandHandler() {
    if(server.arg("cmd") == "") {
        server.send(200, "text/html", FRONTEND);
        return;
    }
    // ---if there are multiple URL params we iterate through them like this---
    // for(int i=0; i<server.args(); i++)
    //     msg += server.argName(i) + ": " + server.arg(i);

     //parse request
    int parsedValues = sscanf(server.arg("cmd").c_str(), "%d-%d", &command, &value);
    if(parsedValues != 1 && parsedValues != 2) {
        server.send(400, "text/plain", "Failed to parse request" + server.arg("cmd"));
        return;
    }
    Serial.println(server.arg("cmd"));
    //validate request then forward to controller
    switch(command) {
        case MOVE_FORWARD:
        case MOVE_BACKWARD:
        case TURN_LEFT:
        case TURN_RIGHT:
        case LEFT_SWEEP:
        case RIGHT_SWEEP:
        case SWEEP_ROUTINE:
        case RAM:
        case STOP:
            server.send(200, "text/plain", server.arg("cmd"));
            Serial1.println(server.arg("cmd"));
            break;
        default: //debug
            server.send(404, "text/plain", "Invalid command" + server.arg("cmd"));
    }
}

/**
 * - Initializes Serial communication for debugging with a baud rate of 115200.
 * - Initializes an HTTP WebServer on port 80
 * - Initilizes the hidden access point with an SSID of `RAM-BOT` and a password of `RAM_RANCH`. 
 *   This access point also has a limit of 1 simultaneously connected client.
 * 
 * (idk if a WebSocket is possible and if so that would be faster but that could be a
 * possible future redesign)
*/
void setup() {
    Serial.begin(BAUD_RATE);
    Serial1.begin(BAUD_RATE);
    WiFi.softAP("RAM-BOT", "RAM_RANCH", 1, 1, 2); //channel 1, hidden, max 2 clients

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.on("/", commandHandler);
    server.begin();
    Serial.println("RAM-BOT HTTP Server and AP Fully Initialized!");
}

/**
 * Infinitely loop to handle HTTP requests to either the frontend or commands
*/
void loop() { server.handleClient(); }