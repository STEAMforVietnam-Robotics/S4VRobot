# S4VRobot

Thư viện Arduino để điều khiển xe robot S4V có cánh tay robot qua Bluetooth, sử dụng ứng dụng [Dabble](https://thestempedia.com/product/dabble/) trên điện thoại thông minh với ESP32.

## Tính năng

- Giao tiếp Bluetooth Low Energy (BLE) qua ứng dụng Dabble
- Điều khiển động cơ DC (tiến, lùi, trái, phải, dừng)
- Hiệu chỉnh tốc độ động cơ trái/phải qua hằng số offset để robot đi thẳng
- Điều khiển servo cánh tay robot (lên/xuống)
- Điều khiển servo kẹp (kẹp/nhả)
- Tích hợp module GamePad để điều khiển bằng điện thoại

## Yêu cầu phần cứng

| Linh kiện | Chi tiết |
|-----------|---------|
| Vi điều khiển | ESP32 |
| Mạch điều khiển động cơ | L298N |
| Động cơ di chuyển | 2x động cơ DC (trái & phải) |
| Servo cánh tay | Kết nối với chân 17 |
| Servo kẹp | Kết nối với chân 18 |
| Ứng dụng | Dabble (iOS / Android) |

## Sơ đồ chân

| Chân | Chức năng |
|-----|---------|
| GPIO 4 | L298N ENA (PWM động cơ trái) |
| GPIO 5 | L298N IN1 |
| GPIO 6 | L298N IN2 |
| GPIO 7 | L298N IN3 |
| GPIO 15 | L298N IN4 |
| GPIO 16 | L298N ENB (PWM động cơ phải) |
| GPIO 17 | Tín hiệu servo cánh tay |
| GPIO 18 | Tín hiệu servo kẹp |

## Cài đặt

1. Tải repository này dưới dạng file `.zip`.
2. Trong Arduino IDE: **Sketch → Include Library → Add .ZIP Library...**
3. Chọn file vừa tải về.
4. Cài đặt thêm thư viện **Dabble** từ Arduino Library Manager.

## Hiệu chỉnh động cơ

Hai động cơ thường có đặc tính cơ khí khác nhau, khiến robot đi lệch. Thư viện cung cấp hai hằng số để bù lại sự chênh lệch này:

| Hằng số | Mặc định | Mô tả |
|---------|---------|-------|
| `LEFT_MOTOR_OFFSET` | `1.0f` | Hệ số tốc độ động cơ trái (0.0 – 1.0) |
| `RIGHT_MOTOR_OFFSET` | `1.0f` | Hệ số tốc độ động cơ phải (0.0 – 1.0) |

Để tùy chỉnh, định nghĩa các hằng số này **trước** khi `#include`:

```cpp
#define LEFT_MOTOR_OFFSET  1.0f
#define RIGHT_MOTOR_OFFSET 0.90f  // Tăng nếu robot lệch trái, giảm nếu lệch phải
#include <S4VRobot.h>
```

## Bắt đầu nhanh

```cpp
#define LEFT_MOTOR_OFFSET  1.0f   // Hệ số tốc độ động cơ trái (0.0 - 1.0)
#define RIGHT_MOTOR_OFFSET 0.85f  // Hệ số tốc độ động cơ phải (0.0 - 1.0)
#include <S4VRobot.h>

S4VRobot Robot;

void setup() {
  Serial.begin(115200);
  Robot.begin("S4VRobot"); // Tên thiết bị BLE hiển thị trong Dabble
}

void loop() {
  Robot.processInput(); // Phải gọi mỗi vòng lặp

  if (Robot.isAppConnected()) {
    if (GamePad.isCrossPressed())      Robot.arm_upward();
    if (GamePad.isTrianglePressed())   Robot.arm_downward();
    if (GamePad.isSquarePressed())     Robot.grasp();
    if (GamePad.isCirclePressed())     Robot.release();

    if (GamePad.isUpPressed())         Robot.go_forward();
    else if (GamePad.isDownPressed())  Robot.go_backward();
    else if (GamePad.isLeftPressed())  Robot.turn_left();
    else if (GamePad.isRightPressed()) Robot.turn_right();
    else                               Robot.stop();
  } else {
    Robot.stop();
  }

  delay(15);
}
```

## Tài liệu API

### Khởi tạo

| Phương thức | Mô tả |
|--------|-------------|
| `begin(name)` | Khởi tạo BLE, động cơ và servo. `name` là tên thiết bị BLE (mặc định: `"S4VRobot"`). |
| `wait_app_connection()` | Chờ cho đến khi ứng dụng Dabble kết nối. |
| `processInput()` | Xử lý dữ liệu BLE đến — gọi mỗi `loop()`. |
| `isAppConnected()` | Trả về `true` nếu ứng dụng Dabble đang kết nối. |

### Điều khiển di chuyển

| Phương thức | Mô tả |
|--------|-------------|
| `go_forward(speed)` | Di chuyển tiến. Tốc độ mặc định: `255` (0–255). |
| `go_backward(speed)` | Di chuyển lùi. |
| `turn_left(speed)` | Quay trái (xoay tại chỗ). |
| `turn_right(speed)` | Quay phải (xoay tại chỗ). |
| `stop()` | Dừng cả hai động cơ. |

### Cánh tay & Kẹp

| Phương thức | Mô tả |
|--------|-------------|
| `arm_upward()` | Nâng cánh tay lên một bước (0–180°). |
| `arm_downward()` | Hạ cánh tay xuống một bước. |
| `grasp()` | Đóng kẹp một bước. |
| `release()` | Mở kẹp một bước. |

## Cài đặt ứng dụng Dabble

1. Cài đặt **Dabble** trên điện thoại ([Google Play](https://play.google.com/store/apps/details?id=io.dabbleapp) / [App Store](https://apps.apple.com/app/dabble-diy-arduino-projects/id1472298547)).
2. Mở ứng dụng → chọn module **GamePad**.
3. Kết nối với thiết bị BLE tên `S4VRobot` (hoặc tên tùy chỉnh của bạn).

## Thư viện phụ thuộc

- [DabbleESP32](https://github.com/STEMpedia/DabbleESP32) — đã được tích hợp trong thư viện này
- [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) — đã được tích hợp trong thư viện này

## Giấy phép

MIT License — xem [LICENSE](LICENSE) để biết chi tiết.

## Tác giả

ThangND — [thang.nd@steamforvietnam.org](mailto:thang.nd@steamforvietnam.org)  
Steam for Vietnam
