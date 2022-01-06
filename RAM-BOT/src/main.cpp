#include <Arduino.h>
#include <string.h>

#include "servo/Servo.h"
#include "motor/Motor.h"
#include "comms/Comms.h"

//current status variables of the system
int command, value, toggleServoRoutine = 0;

/**
 * Initializes UART Serial communication on baud rate 115200. 
*/
void setup() {
    Serial.begin(BAUD_RATE);
    Serial1.begin(BAUD_RATE);
    motorSetup();
    servoSetup();
    Serial.println("RAM-BOT Active");
}

/**
 * Infinite loop to handle commands by UART from the NodeMCU ESP8266
*/
void loop() {
    if(Serial1.available() <= 0)
        if(toggleServoRoutine)
            servoSweepRoutine();
        else
            return;

    int parsedValues = sscanf(Serial1.readStringUntil('\n').c_str(), "%d-%d", &command, &value);
    if(parsedValues != 1 && parsedValues != 2)
        return;

    //handle request
    switch(command) {
        case MOVE_FORWARD:
        case MOVE_BACKWARD:
        case TURN_LEFT:
        case TURN_RIGHT:
        case STOP:
            motorControl(command, value);
            break;
        case LEFT_SWEEP:
        case RIGHT_SWEEP:
            servoSweep(command);
            break;
        case SWEEP_ROUTINE:
            toggleServoRoutine = toggleServoRoutine ? 0 : 1;
            break;
        case RAM:
            ramPunchRoutine();
            break;
    }
}