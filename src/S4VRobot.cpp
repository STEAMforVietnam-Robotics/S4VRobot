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


/*!
    @brief Mô tả: Hàm khởi tạo các thông số cho robot (ví dụ: trạng thái các chân Digital và Analog).
    Hàm này bắt buộc phải được chạy ở hàm setup() trước vòng lặp loop() để có thể điều khiển robot.
    @param name : Tên robot của bạn.
*/
void S4VRobot::begin(std::string name)
{
    Dabble.begin(name);
    this->leds.begin();
    this->leds.show();
    this->leds2.begin();
    this->leds2.show();
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

    preferences.begin("color",false);

    this->led_color = (LedColor)preferences.getUChar("led_color",LED_RED);
    this->mode_cnt = this->led_color;
    this->led_brightness(150);
    this->led_set_color(this->led_color);
    
}
/*!
  @brief   Configure NeoPixel pin for output.
  @returns False if we weren't able to claim resources required
*/
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
    if(GamePad.isSelectPressed())
    {
        if(millis() -  t_start > 2000)
        {
            led_color = (LedColor)(++mode_cnt % 8);
            this->led_set_color(led_color);
            this->preferences.putUChar("led_color", led_color);
            t_start = millis();
        }
    }
    else t_start = millis();
}
/**
 * @brief Mô tả: Hàm điều khiển tay đòn của robot hạ xuống phía dưới.
 * @param step: Bước của cánh tay > 0
 */
void S4VRobot::arm_downward(float step)
{
    if (armAngle < ARM_SERVO_MAX_ANGLE) {
        armAngle += step;
        armServo.write(floor(armAngle));
    }
}
/**
 * @brief Mô tả: Hàm điều khiển tay đòn của robot nâng lên phía trên.
 * @param step:  Bước của cánh tay > 0
 */
void S4VRobot::arm_upward(float step)
{
    if (armAngle > ARM_SERVO_MIN_ANGLE) {
        armAngle -= step;
        armServo.write(ceil(armAngle));
    }
}
/**
 * @brief Mô tả: Hàm điều khiển càng gắp của robot kẹp vào.
 * @param step:  Bước của tay gắp > 0, bước càng lớn tốc độ càng nhanh
 */
void S4VRobot::grasp(float step)
{
    if (gripperAngle < GRIPPER_SERVO_MAX_ANGLE) {
        gripperAngle += step;
        gripperServo.write(floor(gripperAngle));
    }
}
/**
 * @brief Mô tả: Hàm điều khiển càng gắp của robot nhả ra.
 * @param step:  Bước của tay gắp > 0, bước càng lớn tốc độ càng nhanh
 */
void S4VRobot::release(float step)
{
    if (gripperAngle > GRIPPER_SERVO_MIN_ANGLE) {
        gripperAngle -= step;
        gripperServo.write(ceil(gripperAngle));
    }
}
/**
 * @brief Điều khiển trực tiếp một động cơ DC.
 * @param motor     : Động cơ cần điều khiển (LEFT_MOTOR / RIGHT_MOTOR).
 * @param direction : Chiều quay (CLOCKWISE / COUNTER_CLOCKWISE).
 * @param speed     : Tốc độ raw PWM (0–255). Tốc độ thực tế được tăng dần qua calSoftSpeed()
 *                    và nhân với hệ số offset tương ứng của từng động cơ.
 */
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
        analogWrite(L298_ENA, (uint8_t)(currentSpeed * rightMotorSpeedOffset));
    } else if (motor == LEFT_MOTOR) {
        if (direction == CLOCKWISE) {
            digitalWrite(L298_IN3, HIGH);
            digitalWrite(L298_IN4, LOW);
        } else {
            digitalWrite(L298_IN3, LOW);
            digitalWrite(L298_IN4, HIGH);
        }
        analogWrite(L298_ENB, (uint8_t)(currentSpeed * leftMotorSpeedOffset));
    }
}
/**
 * @brief Tính toán tốc độ tăng dần (soft-start) để tránh giật cục khi thay đổi tốc độ.
 *        Mỗi lần gọi, tốc độ hiện tại tăng tối đa MOTOR_STEP đơn vị cho đến khi đạt giá trị mục tiêu.
 * @param speed : Tốc độ mục tiêu (0–255).
 * @return      : Tốc độ thực tế sẽ áp dụng cho lần gọi này.
 */
uint8_t S4VRobot::calSoftSpeed(uint8_t speed)
{
    if((currentSpeed+motor_step) > speed) return speed;
    else return currentSpeed+motor_step;
}

/**
 * @brief Kiểm tra ứng dụng Dabble có đang kết nối với robot hay không.
 * @return true nếu app đang kết nối, false nếu ngược lại.
 */
bool S4VRobot::isAppConnected()
{
    return Dabble.isAppConnected();
}

/**
 * @brief Chuyển LedColor thành giá trị màu 32-bit GRB cho NeoPixel.
 */
uint32_t S4VRobot::_ledColorValue(LedColor color)
{
    switch (color) {
        case LED_RED:     return leds.Color(255,   0,   0);
        case LED_GREEN:   return leds.Color(  0, 255,   0);
        case LED_BLUE:    return leds.Color(  0,   0, 255);
        case LED_YELLOW:  return leds.Color(255, 255,   0);
        case LED_CYAN:    return leds.Color(  0, 255, 255);
        case LED_MAGENTA: return leds.Color(255,   0, 255);
        case LED_WHITE:   return leds.Color(255, 255, 255);
        default:          return leds.Color(  0,   0,   0);
    }
}

/**
 * @brief Đặt toàn bộ dải LED sang màu định sẵn.
 * @param color : Màu cần hiển thị (LED_RED, LED_GREEN, LED_BLUE, ...).
 */
void S4VRobot::led_set_color(LedColor color, LedStrip strip)
{
    uint32_t c = _ledColorValue(color);
    if (strip != LED_STRIP_2) { leds.fill(c);  leds.show();  }
    if (strip != LED_STRIP_1) { leds2.fill(c); leds2.show(); }
}

/**
 * @brief Đặt dải LED sang màu RGB tùy chỉnh.
 * @param r     : Thành phần đỏ (0–255).
 * @param g     : Thành phần xanh lá (0–255).
 * @param b     : Thành phần xanh dương (0–255).
 * @param strip : Dải LED cần điều khiển (mặc định: cả 2).
 */
void S4VRobot::led_set_color(uint8_t r, uint8_t g, uint8_t b, LedStrip strip)
{
    uint32_t c = leds.Color(r, g, b);
    if (strip != LED_STRIP_2) { leds.fill(c);  leds.show();  }
    if (strip != LED_STRIP_1) { leds2.fill(c); leds2.show(); }
}

/**
 * @brief Đặt một LED đơn lẻ sang màu định sẵn.
 * @param index : Chỉ số LED (0 đến NUM_LEDS_x - 1).
 * @param color : Màu cần hiển thị.
 * @param strip : Dải LED cần điều khiển (mặc định: strip 1).
 */
void S4VRobot::led_set_pixel(uint8_t index, LedColor color, LedStrip strip)
{
    uint32_t c = _ledColorValue(color);
    if (strip != LED_STRIP_2) { leds.setPixelColor(index, c);  leds.show();  }
    if (strip != LED_STRIP_1) { leds2.setPixelColor(index, c); leds2.show(); }
}

/**
 * @brief Đặt một LED đơn lẻ sang màu RGB tùy chỉnh.
 * @param index : Chỉ số LED (0 đến NUM_LEDS_x - 1).
 * @param r     : Thành phần đỏ (0–255).
 * @param g     : Thành phần xanh lá (0–255).
 * @param b     : Thành phần xanh dương (0–255).
 * @param strip : Dải LED cần điều khiển (mặc định: strip 1).
 */
void S4VRobot::led_set_pixel(uint8_t index, uint8_t r, uint8_t g, uint8_t b, LedStrip strip)
{
    uint32_t c = leds.Color(r, g, b);
    if (strip != LED_STRIP_2) { leds.setPixelColor(index, c);  leds.show();  }
    if (strip != LED_STRIP_1) { leds2.setPixelColor(index, c); leds2.show(); }
}

/**
 * @brief Tắt dải LED.
 * @param strip : Dải LED cần tắt (mặc định: cả 2).
 */
void S4VRobot::led_off(LedStrip strip)
{
    if (strip != LED_STRIP_2) { leds.clear();  leds.show();  }
    if (strip != LED_STRIP_1) { leds2.clear(); leds2.show(); }
}

/**
 * @brief Điều chỉnh độ sáng dải LED.
 * @param brightness : Độ sáng (0 = tắt, 255 = sáng tối đa).
 * @param strip      : Dải LED cần điều chỉnh (mặc định: cả 2).
 */
void S4VRobot::led_brightness(uint8_t brightness, LedStrip strip)
{
    if (strip != LED_STRIP_2) { leds.setBrightness(brightness);  leds.show();  }
    if (strip != LED_STRIP_1) { leds2.setBrightness(brightness); leds2.show(); }
}