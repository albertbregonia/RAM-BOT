#include "Servo.h"
#include "comms/Comms.h"

Servo leftServoArm, rightServoArm, ramServo;

//left and right arms as well as the battering ram
void servoSetup() {
    leftServoArm.attach(LEFT_ARM_PIN);
    rightServoArm.attach(RIGHT_ARM_PIN);
    ramServo.attach(RAM_SERVO_PIN);
    leftServoArm.write(MIN_ANGLE);
    rightServoArm.write(MIN_ANGLE);
    ramServo.write(MIN_ANGLE);
}

void servoSweepRoutine() {
    int i = 0;
    for(; i<MAX_ANGLE; i++) {
        rightServoArm.write(i);
        if(i > MID_ANGLE)
            leftServoArm.write(i-MID_ANGLE);
        delay(SWEEP_SPEED);
    }
    for(; i>MIN_ANGLE; i--) {
        rightServoArm.write(i);
        leftServoArm.write(i > MID_ANGLE ? (MAX_ANGLE-i) + MID_ANGLE : i+MID_ANGLE);
        delay(SWEEP_SPEED);
    }
    for(i=MID_ANGLE; i>MIN_ANGLE; i--) {
        leftServoArm.write(i);
        delay(SWEEP_SPEED);
    }
}

void servoSweep(int command) {
    Servo arm = (command == LEFT_SWEEP) ? leftServoArm : rightServoArm; 
    int i = MIN_ANGLE;
    for(; i<MAX_ANGLE; i++) {
        arm.write(i);
        delay(SWEEP_SPEED);
    }
    for(; i>=MIN_ANGLE; i--) {
        arm.write(i);
        delay(SWEEP_SPEED);
    }
}

void ramPunchRoutine() {
    ramServo.write(MAX_ANGLE);
    delay(500);
    ramServo.write(0);
}
