/**
 * @file S4VRobot.h
 * @brief Khai báo các hàm điều khiển cho Robot S4V và link tới thư viện Dabble App
 * @author Manh Vu
 * * @note 
 * LAST UPDATE: 2024-05-20 11:1PM
 * - Thêm Function Description cho tất cả các hàm
 * * @version 1.2.0
 */

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
        /**
         * @brief Mô tả: Hàm khởi tạo các thông số cho robot (ví dụ: trạng thái các chân Digital và Analog).
         * Hàm này bắt buộc phải được chạy ở hàm setup() trước vòng lặp loop() để có thể điều khiển robot.
         * @param name : Tên robot của bạn.
         */
        void begin(std::string name = "S4VRobot");
        void wait();

        /**
         * @brief Mô tả: Hàm điều khiển robot dừng lại.
         */
        void stop();

        /**
         * @brief Mô tả: Hàm điều khiển robot chạy thẳng.
         * @param speed: Tốc độ di chuyển của robot (0 - 100%).
         */
        void go_forward(uint8_t speed = MOTOR_SPEED);

        
        /**
         * @brief Mô tả: Hàm điều khiển robot chạy lùi.
         * @param speed: Tốc độ di chuyển của robot (0 - 100%).
         */
        void go_backward(uint8_t speed = MOTOR_SPEED);

        /**
         * @brief Mô tả: Hàm điều khiển robot xoay trái.
         * @param speed: Tốc độ xoay trái của robot (0 - 100%).
         */
        void turn_left(uint8_t speed = MOTOR_SPEED);

        /**
         * @brief Mô tả: Hàm điều khiển robot xoay phải.
         * @param speed: Tốc độ xoay phải của robot (0 - 100%).
         */
        void turn_right(uint8_t speed = MOTOR_SPEED);

        /**
         * @brief Mô tả: Hàm điều khiển càng gắp của robot thu lại.
         */
        void grasp();

        /**
         * @brief Mô tả: Hàm điều khiển càng gắp của robot mở ra.
         */
        void release();

        /**
         * @brief Mô tả: Hàm kiểm tra tín hiệu điều khiển từ app Dabble trên điện thoại.
         */
        void processInput();

        /**
         * @brief Mô tả: Hàm điều khiển tay đòn của robot nâng lên phía trên.
         */
        void arm_upward();

        /**
         * @brief Mô tả: Hàm điều khiển tay đòn của robot hạ xuống phía dưới.
         */
        void arm_downward();
        void wait_app_connection();
    private:
        int32_t armAngle;
        Servo armServo;
        Servo gripperServo;
        int32_t gripperAngle;

        /**
         * @brief Mô tả: Hàm nội bộ điều khiển riêng cho động cơ.
         * @param motor: Motor bên trái (LEFT_MOTOR) hoặc motor bên phải (RIGHT_MOTOR).
         * @param direction: Hướng xoay động cơ thuận chiều kim đồng hồ (CLOCKWISE) hay ngược chiều kim đồng hồ (COUNTER_CLOCKWISE).
         * @param speed: Tốc độ của động cơ (0 - 100%).
         */
        void control_motor(Motor motor, Direction direction, uint8_t speed);
};

#endif