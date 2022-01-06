#define LEFT_MOTORS_VCC 7
#define LEFT_MOTORS_GND 6
#define RIGHT_MOTORS_VCC 5
#define RIGHT_MOTORS_GND 4
#define MOTOR_SPEED 3

void motorSetup();
void motorControl(int command, int value);