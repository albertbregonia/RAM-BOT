#include <Servo.h>
#include <Arduino.h>

#define LEFT_ARM_PIN 10
#define RIGHT_ARM_PIN 9
#define RAM_SERVO_PIN 8

#define MIN_ANGLE 10
#define MAX_ANGLE 170
#define MID_ANGLE MAX_ANGLE/2
#define SWEEP_SPEED 500/MAX_ANGLE

void servoSetup();
void servoSweepRoutine();
void servoSweep(int command);
void ramPunchRoutine();