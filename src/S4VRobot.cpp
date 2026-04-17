#define FROM_DABBLE_LIBRARY
#include "S4VRobot.h"



S4VRobot::S4VRobot() : armAngle(ARM_SERVO_MIN_ANGLE), gripperAngle(GRIPPER_SERVO_MIN_ANGLE) {};

void S4VRobot::begin(std::string name)
{
    Dabble.begin(name);
    this->armServo.attach(ARM_SERVO_PIN);
    this->gripperServo.attach(GRIPPER_SERVO_PIN);
    this->armServo.write(armAngle);
    this->gripperServo.write(gripperAngle);
    pinMode(L298_ENA, OUTPUT);
    pinMode(L298_IN1, OUTPUT);
    pinMode(L298_IN2, OUTPUT);
    pinMode(L298_IN3, OUTPUT);
    pinMode(L298_IN4, OUTPUT);
    pinMode(L298_ENB, OUTPUT);

    // Set initial state of the motors
    digitalWrite(L298_IN1, LOW);
    digitalWrite(L298_IN2, LOW);
    digitalWrite(L298_IN3, LOW);
    digitalWrite(L298_IN4, LOW);

    // Set initial speed of the motors
    analogWrite(L298_ENA, MOTOR_SPEED); // Full speed for motor A
    analogWrite(L298_ENB, MOTOR_SPEED); // Full speed for motor B
}

void S4VRobot::wait()
{
    // Wait for the app to connect
    delay(1000);
}

void S4VRobot::stop()
{
    // Stop the motors
    control_motor(LEFT_MOTOR, CLOCKWISE, 0);
    control_motor(RIGHT_MOTOR, CLOCKWISE, 0);
}

void S4VRobot::go_forward(uint8_t speed)
{
    // Move forward
    control_motor(LEFT_MOTOR, CLOCKWISE, speed);
    control_motor(RIGHT_MOTOR, CLOCKWISE, speed);
}

void S4VRobot::go_backward(uint8_t speed)
{
    // Move backward
    control_motor(LEFT_MOTOR, COUNTER_CLOCKWISE, speed);
    control_motor(RIGHT_MOTOR, COUNTER_CLOCKWISE, speed);
}

void S4VRobot::turn_left(uint8_t speed)
{
    // Turn left
    control_motor(LEFT_MOTOR, COUNTER_CLOCKWISE, speed);
    control_motor(RIGHT_MOTOR, CLOCKWISE, speed);
}

void S4VRobot::turn_right(uint8_t speed)
{
    // Turn right
    control_motor(LEFT_MOTOR, CLOCKWISE, speed);
    control_motor(RIGHT_MOTOR, COUNTER_CLOCKWISE, speed);
}

void S4VRobot::wait_app_connection()
{
    Dabble.waitForAppConnection();
}

void S4VRobot::processInput()
{
    Dabble.processInput();
}

void S4VRobot::arm_upward()
{
    if (armAngle < ARM_SERVO_MAX_ANGLE) {
        armAngle += ARM_SERVO_STEP;
        armServo.write(armAngle);
    }
}

void S4VRobot::arm_downward()
{
    if (armAngle > ARM_SERVO_MIN_ANGLE) {
        armAngle -= ARM_SERVO_STEP;
        armServo.write(armAngle);
    }
}


void S4VRobot::grasp()
{
    if (gripperAngle < GRIPPER_SERVO_MAX_ANGLE) {
        gripperAngle += GRIPPER_SERVO_STEP;
        gripperServo.write(gripperAngle);
    }
}

void S4VRobot::release()
{
    if (gripperAngle > GRIPPER_SERVO_MIN_ANGLE) {
        gripperAngle -= GRIPPER_SERVO_STEP;
        gripperServo.write(gripperAngle);
    }
}

void S4VRobot::control_motor(Motor motor, Direction direction, uint8_t speed)
{
    if (motor == LEFT_MOTOR) {
        if (direction == CLOCKWISE) {
            digitalWrite(L298_IN1, HIGH);
            digitalWrite(L298_IN2, LOW);
        } else {
            digitalWrite(L298_IN1, LOW);
            digitalWrite(L298_IN2, HIGH);
        }
        analogWrite(L298_ENA, speed);
    } else if (motor == RIGHT_MOTOR) {
        if (direction == CLOCKWISE) {
            digitalWrite(L298_IN3, HIGH);
            digitalWrite(L298_IN4, LOW);
        } else {
            digitalWrite(L298_IN3, LOW);
            digitalWrite(L298_IN4, HIGH);
        }
        analogWrite(L298_ENB, speed);
    }
}