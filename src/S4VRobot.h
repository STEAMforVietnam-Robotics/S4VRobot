#ifndef S4V_ROBOT_H
#define S4V_ROBOT_H

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include "DabbleESP32.h"
#include "ESP32Servo.h"

#define L298_ENA 4
#define L298_IN1 5
#define L298_IN2 6
#define L298_IN3 7
#define L298_IN4 15
#define L298_ENB 16

#define MOTOR_SPEED 150

#define ARM_SERVO_PIN 17
#define ARM_SERVO_MIN_ANGLE 0
#define ARM_SERVO_MAX_ANGLE 180
#define ARM_SERVO_STEP 1

#define GRIPPER_SERVO_PIN 18
#define GRIPPER_SERVO_MIN_ANGLE 0
#define GRIPPER_SERVO_MAX_ANGLE 180
#define GRIPPER_SERVO_STEP 1



typedef enum {
    CLOCKWISE = 0,
    COUNTER_CLOCKWISE = 1
} Direction;

typedef enum {
    LEFT_MOTOR = 0,
    RIGHT_MOTOR = 1,
    ARM_MOTOR = 2
} Motor;

class S4VRobot
{
    public:
        S4VRobot();
        void begin(std::string name = "S4VRobot");
        void wait();
        void stop();
        void go_forward(uint8_t speed = MOTOR_SPEED);
        void go_backward(uint8_t speed = MOTOR_SPEED);
        void turn_left(uint8_t speed = MOTOR_SPEED);
        void turn_right(uint8_t speed = MOTOR_SPEED);
        void grasp();
        void release();
        void processInput();
        void arm_upward();
        void arm_downward();
        void wait_app_connection();
    private:
        int32_t armAngle;
        Servo armServo;
        Servo gripperServo;
        int32_t gripperAngle;
        void control_motor(Motor motor, Direction direction, uint8_t speed);
};

#endif