/**
 * @file S4VRobot.h
 * @brief Khai báo các hàm điều khiển cho Robot S4V và link tới thư viện Dabble App
 * @author Manh Vu
 * * @note
 * LAST UPDATE: 2024-05-20 11:18PM
 * - Xóa hằng số MOTOR_SPEED
 * - Hàm khởi tạo robot đặt tốc độ ban đầu của động cơ bằng 0
 * * @version 1.2.0
 */

#ifndef S4V_ROBOT_H
#define S4V_ROBOT_H

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include "DabbleESP32.h"
#include "ESP32Servo.h"
#include <Adafruit_NeoPixel.h>


#define L298_ENA 4
#define L298_IN1 5
#define L298_IN2 6
#define L298_IN3 7
#define L298_IN4 15
#define L298_ENB 16

#ifndef MOTOR_STEP
#define MOTOR_STEP 8
#endif

#define ARM_SERVO_PIN 17
#define ARM_SERVO_MIN_ANGLE 0
#define ARM_SERVO_MAX_ANGLE 180
#define ARM_SERVO_DEFAULT_ANGLE ARM_SERVO_MAX_ANGLE

#define GRIPPER_SERVO_PIN 18
#define GRIPPER_SERVO_MIN_ANGLE 70
#define GRIPPER_SERVO_MAX_ANGLE 140
#define GRIPPER_SERVO_DEFAULT_ANGLE GRIPPER_SERVO_MAX_ANGLE

#ifndef LED_PIN_1
#define LED_PIN_1 12
#endif

#ifndef NUM_LEDS_1
#define NUM_LEDS_1 24
#endif

#ifndef LED_PIN_2
#define LED_PIN_2 13
#endif

#ifndef NUM_LEDS_2
#define NUM_LEDS_2 24
#endif

#ifndef LEFT_MOTOR_OFFSET
#define LEFT_MOTOR_OFFSET  1.0f   // Hệ số tốc độ động cơ trái (0.0 - 1.0)
#endif

#ifndef RIGHT_MOTOR_OFFSET
#define RIGHT_MOTOR_OFFSET 1.0f  // Hệ số tốc độ động cơ phải (0.0 - 1.0)
#endif

typedef enum {
    CLOCKWISE = 0,
    COUNTER_CLOCKWISE = 1
} Direction;

typedef enum {
    LEFT_MOTOR = 0,
    RIGHT_MOTOR = 1,
    ARM_MOTOR = 2
} Motor;

typedef enum {
    LED_RED     = 0,
    LED_BLUE    = 1,
    LED_GREEN   = 2,
    LED_YELLOW  = 3,
    LED_CYAN    = 4,
    LED_MAGENTA = 5,
    LED_WHITE   = 6,
    LED_OFF     = 7
} LedColor;

typedef enum {
    LED_STRIP_ALL = 0,
    LED_STRIP_1   = 1,
    LED_STRIP_2   = 2
} LedStrip;

class S4VRobot
{
    public:
        S4VRobot(){};
        void begin(std::string name = "S4VRobot");
        void wait();
        void stop();
        void go_forward(uint8_t speed = 100);
        void go_backward(uint8_t speed = 100);
        void turn_left(uint8_t speed = 80);
        void turn_right(uint8_t speed = 80);
        void grasp(float step = 1.0); 
        void release(float step = 1.0);
        void processInput();
        void arm_upward(float step = 1.0);
        void arm_downward(float step = 1.0);
        void wait_app_connection();
        bool isAppConnected();
        void control_motor(Motor motor, Direction direction, uint8_t speed);
        void led_set_color(LedColor color, LedStrip strip = LED_STRIP_ALL);
        void led_set_color(uint8_t r, uint8_t g, uint8_t b, LedStrip strip = LED_STRIP_ALL);
        void led_set_pixel(uint8_t index, LedColor color, LedStrip strip = LED_STRIP_1);
        void led_set_pixel(uint8_t index, uint8_t r, uint8_t g, uint8_t b, LedStrip strip = LED_STRIP_1);
        void led_off(LedStrip strip = LED_STRIP_ALL);
        void led_brightness(uint8_t brightness, LedStrip strip = LED_STRIP_ALL);

    private:
        float armAngle = ARM_SERVO_DEFAULT_ANGLE;
        Servo armServo;
        Servo gripperServo;
        float gripperAngle = GRIPPER_SERVO_DEFAULT_ANGLE;
        uint8_t currentSpeed = 0;
        uint8_t calSoftSpeed(uint8_t speed);
        float leftMotorSpeedOffset = LEFT_MOTOR_OFFSET;
        float rightMotorSpeedOffset = RIGHT_MOTOR_OFFSET;
        Adafruit_NeoPixel leds  = Adafruit_NeoPixel(NUM_LEDS_1, LED_PIN_1, NEO_GRB + NEO_KHZ800);
        Adafruit_NeoPixel leds2 = Adafruit_NeoPixel(NUM_LEDS_2, LED_PIN_2, NEO_GRB + NEO_KHZ800);
        uint32_t _ledColorValue(LedColor color);
        uint32_t t_start = 0; 
        LedColor led_color = LED_RED;
        uint8_t motor_step = MOTOR_STEP;
};

#endif
