#include <Arduino.h>

#include "Motor.h"
#include "comms/Comms.h"

void motorSetup() {
    int motorPins[] = {
        LEFT_MOTORS_VCC,
        LEFT_MOTORS_GND,
        RIGHT_MOTORS_VCC,
        RIGHT_MOTORS_GND,
        MOTOR_SPEED,
    };
    for(int i=0, pin; i<sizeof(motorPins)/sizeof(int); i++) {
        pin = motorPins[i];
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
}

void motorControl(int command, int value) {
    analogWrite(MOTOR_SPEED, value);
    switch(command) {
        case MOVE_FORWARD:
            digitalWrite(LEFT_MOTORS_VCC, HIGH);
            digitalWrite(RIGHT_MOTORS_VCC, HIGH);
            digitalWrite(LEFT_MOTORS_GND, LOW);
            digitalWrite(RIGHT_MOTORS_GND, LOW);
            break;
        case MOVE_BACKWARD:
            digitalWrite(LEFT_MOTORS_VCC, LOW);
            digitalWrite(RIGHT_MOTORS_VCC, LOW);
            digitalWrite(LEFT_MOTORS_GND, HIGH);
            digitalWrite(RIGHT_MOTORS_GND, HIGH);
            break;
        case TURN_LEFT:
            digitalWrite(LEFT_MOTORS_VCC, LOW);
            digitalWrite(LEFT_MOTORS_GND, HIGH);
            digitalWrite(RIGHT_MOTORS_VCC, HIGH);
            digitalWrite(RIGHT_MOTORS_GND, LOW);
            break;
        case TURN_RIGHT:
            digitalWrite(LEFT_MOTORS_VCC, HIGH);
            digitalWrite(LEFT_MOTORS_GND, LOW);
            digitalWrite(RIGHT_MOTORS_VCC, LOW);
            digitalWrite(RIGHT_MOTORS_GND, HIGH);
            break;
        case STOP:
            digitalWrite(LEFT_MOTORS_VCC, LOW);
            digitalWrite(LEFT_MOTORS_GND, LOW);
            digitalWrite(RIGHT_MOTORS_VCC, LOW);
            digitalWrite(RIGHT_MOTORS_GND, LOW);
            break;
    }
}

