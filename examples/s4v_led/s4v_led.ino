#define LEFT_MOTOR_OFFSET  1.0f   // Hệ số tốc độ động cơ trái (0.0 - 1.0)
#define RIGHT_MOTOR_OFFSET 1.0f  // Hệ số tốc độ động cơ phải (0.0 - 1.0)
#define NUM_LEDS_1 24
#define NUM_LEDS_2 24
#include <S4VRobot.h>

// Tạo 1 robot mới để điều khiển
S4VRobot Robot;

void setup() {
    Serial.begin(115200);
    // Thiết lập thông số cho robot vừa tạo
    Robot.begin("S4VRobot");
}

void loop()
{
    // Đọc tín hiệu điều khiển từ App trên điện thoại
    Robot.processInput();

    if (Robot.isAppConnected())
    {
        // Nếu nút hình dấu 'X' được nhấn
        if (GamePad.isCrossPressed())
        {
            // Hạ tay đòn của robot xuống phía dưới
            Robot.arm_downward();
        }

        // Nếu nút hình 'TAM GIÁC' được nhấn
        if (GamePad.isTrianglePressed())
        {
            // Nâng tay đòn của robot lên phía trên
            Robot.arm_upward();
        }

        // Nếu nút hình 'VUÔNG' được nhấn
        if(GamePad.isSquarePressed())
        {
            // Thu càng gắp của robot lại
            Robot.grasp();
        }

        // Nếu nút hình 'VÒNG TRÒN' được nhấn
        if(GamePad.isCirclePressed())
        {
            // Mở càng gắp của robot ra
            Robot.release();
        }

        // Nếu nút 'TRÊN' được nhấn
        if(GamePad.isUpPressed())
        {
            // Robot đi thẳng
            Robot.go_forward(100);
        }
        // Nếu nút 'DƯỚI' được nhấn
        else if(GamePad.isDownPressed())
        {
            // Robot đi lùi
            Robot.go_backward(100);
        }
        // Nếu nút 'TRÁI' được nhấn
        else if(GamePad.isLeftPressed())
        {
            // Robot xoay trái
            Robot.turn_left(80);
        }
        // Nếu nút 'PHẢI' được nhấn
        else if(GamePad.isRightPressed())
        {
            // Robot xoay phải
            Robot.turn_right(80);
        }
        // Nếu tất cả điều kiện nút nhấn bên trên đều không thỏa mãn
        else
        {
            // Robot dừng lại
            Robot.stop();
        }
    }
    else
    {
        Robot.stop();
    }

    // Cứ mỗi 10 mili giây sẽ kiểm tra tín hiệu điều khiển từ App trên điện thoại một lần
    delay(10);
}
