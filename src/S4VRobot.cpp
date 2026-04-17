/**
 * @file S4VRobot.cpp
 * @brief Định nghĩa các hàm điều khiển cho Robot S4V và link tới thư viện Dabble App
 * @author Manh Vu
 * * @note 
 * LAST UPDATE: 2024-05-20 10:40PM
 * - Thay đổi range parameter 'speed' cho các hàm điều khiển robot về dạng 0 - 100%
 * - Thêm hàm map() parameter 'speed' về range 0 - 255 bên trong hàm (local scope) để thực hiện băm PWM
 * - Thêm Function Description cho tất cả các hàm
 * * @version 1.2.0
 */

#define FROM_DABBLE_LIBRARY
#include "S4VRobot.h"

S4VRobot::S4VRobot() : armAngle(ARM_SERVO_MIN_ANGLE), gripperAngle(GRIPPER_SERVO_MIN_ANGLE) {};

/**
 * @brief Mô tả: Hàm khởi tạo các thông số cho robot (ví dụ: trạng thái các chân Digital và Analog).
 * Hàm này bắt buộc phải được chạy ở hàm setup() trước vòng lặp loop() để có thể điều khiển robot.
 * @param name : Tên robot của bạn.
 */
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

    analogWrite(L298_ENA, MOTOR_SPEED); 
    analogWrite(L298_ENB, MOTOR_SPEED); 
}

void S4VRobot::wait()
{
    //Chờ Dabble app thiết lập xong
    delay(1000);
}

/**
 * @brief Mô tả: Hàm điều khiển robot dừng lại.
 */
void S4VRobot::stop()
{
    control_motor(LEFT_MOTOR, CLOCKWISE, 0);
    control_motor(RIGHT_MOTOR, CLOCKWISE, 0);
}

/**
 * @brief Mô tả: Hàm điều khiển robot chạy thẳng.
 * @param speed: Tốc độ di chuyển của robot (0 - 100%).
 */
void S4VRobot::go_forward(uint8_t speed)
{
    uint8_t mappedSpeed = map(speed, 0, 100, 0, 255);
    control_motor(LEFT_MOTOR, CLOCKWISE, mappedSpeed);
    control_motor(RIGHT_MOTOR, CLOCKWISE, mappedSpeed);
}

/**
 * @brief Mô tả: Hàm điều khiển robot chạy lùi.
 * @param speed: Tốc độ di chuyển của robot (0 - 100%).
 */
void S4VRobot::go_backward(uint8_t speed)
{
    uint8_t mappedSpeed = map(speed, 0, 100, 0, 255);
    control_motor(LEFT_MOTOR, COUNTER_CLOCKWISE, mappedSpeed);
    control_motor(RIGHT_MOTOR, COUNTER_CLOCKWISE, mappedSpeed);
}

/**
 * @brief Mô tả: Hàm điều khiển robot xoay trái.
 * @param speed: Tốc độ xoay trái của robot (0 - 100%).
 */
void S4VRobot::turn_left(uint8_t speed)
{
    uint8_t mappedSpeed = map(speed, 0, 100, 0, 255);
    control_motor(LEFT_MOTOR, COUNTER_CLOCKWISE, mappedSpeed);
    control_motor(RIGHT_MOTOR, CLOCKWISE, mappedSpeed);
}

/**
 * @brief Mô tả: Hàm điều khiển robot xoay phải.
 * @param speed: Tốc độ xoay phải của robot (0 - 100%).
 */
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

/**
 * @brief Mô tả: Hàm kiểm tra tín hiệu điều khiển từ app Dabble trên điện thoại.
 */
void S4VRobot::processInput()
{
    Dabble.processInput();
}

/**
 * @brief Mô tả: Hàm điều khiển tay đòn của robot nâng lên phía trên.
 */
void S4VRobot::arm_upward()
{
    if (armAngle < ARM_SERVO_MAX_ANGLE) {
        armAngle += ARM_SERVO_STEP;
        armServo.write(armAngle);
    }
}

/**
 * @brief Mô tả: Hàm điều khiển tay đòn của robot hạ xuống phía dưới.
 */
void S4VRobot::arm_downward()
{
    if (armAngle > ARM_SERVO_MIN_ANGLE) {
        armAngle -= ARM_SERVO_STEP;
        armServo.write(armAngle);
    }
}

/**
 * @brief Mô tả: Hàm điều khiển càng gắp của robot thu lại.
 */
void S4VRobot::grasp()
{
    if (gripperAngle < GRIPPER_SERVO_MAX_ANGLE) {
        gripperAngle += GRIPPER_SERVO_STEP;
        gripperServo.write(gripperAngle);
    }
}

/**
 * @brief Mô tả: Hàm điều khiển càng gắp của robot mở ra.
 */
void S4VRobot::release()
{
    if (gripperAngle > GRIPPER_SERVO_MIN_ANGLE) {
        gripperAngle -= GRIPPER_SERVO_STEP;
        gripperServo.write(gripperAngle);
    }
}

/**
 * @brief Mô tả: Hàm nội bộ điều khiển riêng cho động cơ.
 * @param motor: Motor bên trái (LEFT_MOTOR) hoặc motor bên phải (RIGHT_MOTOR).
 * @param direction: Hướng xoay động cơ thuận chiều kim đồng hồ (CLOCKWISE) hay ngược chiều kim đồng hồ (COUNTER_CLOCKWISE).
 * @param speed: Tốc độ của động cơ (0 - 100%).
 */
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