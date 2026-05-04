/**
 * @file S4VRobot.cpp
 * @brief Định nghĩa các hàm điều khiển cho Robot S4V và link tới thư viện Dabble App
 * @author Manh Vu
 * * @note 
 * LAST UPDATE: 2024-05-20 10:40PM
 * - Thay đổi range parameter 'speed' cho các hàm điều khiển robot về dạng 0 - 100%
 * - Thêm hàm map() parameter 'speed' về range 0 - 255 bên trong hàm (local scope) để thực hiện băm PWM
 * * @version 1.2.0
 */

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

    // Thiết lập trạng thái ban đầu của các chân điều khiển động cơ ở mức LOW -> Động cơ không xoay.
    digitalWrite(L298_IN1, LOW);
    digitalWrite(L298_IN2, LOW);
    digitalWrite(L298_IN3, LOW);
    digitalWrite(L298_IN4, LOW);

    analogWrite(L298_ENA, 0); 
    analogWrite(L298_ENB, 0); 
}

void S4VRobot::wait()
{
    //Chờ Dabble app thiết lập xong
    delay(1000);
}

void S4VRobot::stop()
{
    control_motor(LEFT_MOTOR, CLOCKWISE, 0);
    control_motor(RIGHT_MOTOR, CLOCKWISE, 0);
}

void S4VRobot::go_forward(uint8_t speed)
{
    uint8_t mappedSpeed = map(speed, 0, 100, 0, 255);
    control_motor(LEFT_MOTOR, CLOCKWISE, mappedSpeed);
    control_motor(RIGHT_MOTOR, CLOCKWISE, mappedSpeed);
}

void S4VRobot::go_backward(uint8_t speed)
{
    uint8_t mappedSpeed = map(speed, 0, 100, 0, 255);
    control_motor(LEFT_MOTOR, COUNTER_CLOCKWISE, mappedSpeed);
    control_motor(RIGHT_MOTOR, COUNTER_CLOCKWISE, mappedSpeed);
}

void S4VRobot::turn_left(uint8_t speed)
{
    uint8_t mappedSpeed = map(speed, 0, 100, 0, 255);
    control_motor(LEFT_MOTOR, COUNTER_CLOCKWISE, mappedSpeed);
    control_motor(RIGHT_MOTOR, CLOCKWISE, mappedSpeed);
}

void S4VRobot::turn_right(uint8_t speed)
{
    uint8_t mappedSpeed = map(speed, 0, 100, 0, 255);
    control_motor(LEFT_MOTOR, CLOCKWISE, mappedSpeed);
    control_motor(RIGHT_MOTOR, COUNTER_CLOCKWISE, mappedSpeed);
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
    currentSpeed = calSoftSpeed(speed);
    if (motor == RIGHT_MOTOR) {
        if (direction == CLOCKWISE) {
            digitalWrite(L298_IN1, HIGH);
            digitalWrite(L298_IN2, LOW);
        } else {
            digitalWrite(L298_IN1, LOW);
            digitalWrite(L298_IN2, HIGH);
        }
        analogWrite(L298_ENA, (uint8_t)(currentSpeed * RIGHT_MOTOR_OFFSET));
    } else if (motor == LEFT_MOTOR) {
        if (direction == CLOCKWISE) {
            digitalWrite(L298_IN3, HIGH);
            digitalWrite(L298_IN4, LOW);
        } else {
            digitalWrite(L298_IN3, LOW);
            digitalWrite(L298_IN4, HIGH);
        }
        analogWrite(L298_ENB, (uint8_t)(currentSpeed * LEFT_MOTOR_OFFSET));
    }
}

uint8_t S4VRobot::calSoftSpeed(uint8_t speed)
{
    if((currentSpeed+MOTOR_STEP) > speed) return speed;
    else return currentSpeed+MOTOR_STEP;
}

bool S4VRobot::isAppConnected()
{
    return Dabble.isAppConnected();
}