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
#define ARM_SERVO_STEP 1
#define ARM_SERVO_DEFAULT_ANGLE ARM_SERVO_MAX_ANGLE

#define GRIPPER_SERVO_PIN 18
#define GRIPPER_SERVO_MIN_ANGLE 0
#define GRIPPER_SERVO_MAX_ANGLE 90
#define GRIPPER_SERVO_STEP 1
#define GRIPPER_SERVO_DEFAULT_ANGLE GRIPPER_SERVO_MAX_ANGLE

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
        void go_forward(uint8_t speed);


        /**
         * @brief Mô tả: Hàm điều khiển robot chạy lùi.
         * @param speed: Tốc độ di chuyển của robot (0 - 100%).
         */
        void go_backward(uint8_t speed);

        /**
         * @brief Mô tả: Hàm điều khiển robot xoay trái.
         * @param speed: Tốc độ xoay trái của robot (0 - 100%).
         */
        void turn_left(uint8_t speed);

        /**
         * @brief Mô tả: Hàm điều khiển robot xoay phải.
         * @param speed: Tốc độ xoay phải của robot (0 - 100%).
         */
        void turn_right(uint8_t speed);

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
        bool isAppConnected();
        void control_motor(Motor motor, Direction direction, uint8_t speed);
    private:
        int32_t armAngle;
        Servo armServo;
        Servo gripperServo;
        int32_t gripperAngle;
        uint8_t currentSpeed = 0;
        uint8_t calSoftSpeed(uint8_t speed);
};

#endif
