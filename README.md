# S4VRobot

Thư viện Arduino để điều khiển xe robot S4V có cánh tay robot qua Bluetooth, sử dụng ứng dụng [Dabble](https://thestempedia.com/product/dabble/) trên điện thoại thông minh với ESP32.

## Tính năng

- Giao tiếp Bluetooth Low Energy (BLE) qua ứng dụng Dabble
- Điều khiển động cơ DC (tiến, lùi, trái, phải, dừng)
- Tăng tốc mềm (soft-speed) để tránh giật cục khi thay đổi tốc độ đột ngột
- Hiệu chỉnh tốc độ động cơ trái/phải qua hằng số offset để robot đi thẳng
- Điều khiển servo cánh tay robot (lên/xuống theo từng bước)
- Điều khiển servo kẹp (kẹp/nhả theo từng bước)
- Điều khiển **2 dải LED WS2812** độc lập với các màu cơ bản và màu RGB tùy chỉnh
- Tích hợp module GamePad để điều khiển bằng điện thoại

## Yêu cầu phần cứng

| Linh kiện | Chi tiết |
|-----------|---------|
| Vi điều khiển | ESP32 |
| Mạch điều khiển động cơ | L298N |
| Động cơ di chuyển | 2x động cơ DC (trái & phải) |
| Servo cánh tay | Kết nối với chân 17 |
| Servo kẹp | Kết nối với chân 18 |
| Dải LED 1 | WS2812 / NeoPixel — kết nối chân data với GPIO 12 |
| Dải LED 2 | WS2812 / NeoPixel — kết nối chân data với GPIO 13 |
| Ứng dụng | Dabble (iOS / Android) |

## Sơ đồ chân

| Chân | Chức năng |
|-----|---------|
| GPIO 4 | L298N ENA (PWM động cơ **phải**) |
| GPIO 5 | L298N IN1 (chiều động cơ phải) |
| GPIO 6 | L298N IN2 (chiều động cơ phải) |
| GPIO 7 | L298N IN3 (chiều động cơ trái) |
| GPIO 15 | L298N IN4 (chiều động cơ trái) |
| GPIO 16 | L298N ENB (PWM động cơ **trái**) |
| GPIO 17 | Tín hiệu servo cánh tay |
| GPIO 18 | Tín hiệu servo kẹp |
| GPIO 12 | Data dải LED 1 (WS2812) |
| GPIO 13 | Data dải LED 2 (WS2812) |

## Cài đặt

1. Tải repository này dưới dạng file `.zip`.
2. Trong Arduino IDE: **Sketch → Include Library → Add .ZIP Library...**
3. Chọn file vừa tải về.

## Tùy chỉnh

Tất cả các hằng số cấu hình phần cứng đều có thể ghi đè bằng cách định nghĩa **trước** dòng `#include <S4VRobot.h>`:

### Hiệu chỉnh động cơ

Hai động cơ thường có đặc tính cơ khí khác nhau, khiến robot đi lệch. Thư viện cung cấp hai hằng số để bù lại sự chênh lệch này:

| Hằng số | Mặc định | Mô tả |
|---------|---------|-------|
| `LEFT_MOTOR_OFFSET` | `1.0f` | Hệ số tốc độ động cơ trái (0.0 – 1.0) |
| `RIGHT_MOTOR_OFFSET` | `1.0f` | Hệ số tốc độ động cơ phải (0.0 – 1.0) |

```cpp
#define LEFT_MOTOR_OFFSET  1.0f
#define RIGHT_MOTOR_OFFSET 0.90f  // Tăng nếu robot lệch trái, giảm nếu lệch phải
#include <S4VRobot.h>
```

### Tăng tốc mềm (Soft-Speed)

Mỗi lần gọi hàm điều khiển động cơ, tốc độ thực tế không nhảy ngay lên giá trị mong muốn mà tăng dần theo từng bước `MOTOR_STEP` để tránh giật cục. Mặc định `MOTOR_STEP = 8`.

```cpp
#define MOTOR_STEP 15  // Bước tăng tốc lớn hơn → tăng tốc nhanh hơn
#include <S4VRobot.h>
```

### Cấu hình LED WS2812

| Hằng số | Mặc định | Mô tả |
|---------|---------|-------|
| `LED_PIN_1` | `12` | Chân data dải LED 1 |
| `NUM_LEDS_1` | `20` | Số LED trên dải 1 |
| `LED_PIN_2` | `13` | Chân data dải LED 2 |
| `NUM_LEDS_2` | `20` | Số LED trên dải 2 |

```cpp
#define LED_PIN_1   14
#define NUM_LEDS_1  20
#define LED_PIN_2   13
#define NUM_LEDS_2   20
#include <S4VRobot.h>
```

## Bắt đầu nhanh

```cpp
#define LEFT_MOTOR_OFFSET  1.0f   // Hệ số tốc độ động cơ trái (0.0 - 1.0)
#define RIGHT_MOTOR_OFFSET 1.0f   // Hệ số tốc độ động cơ phải (0.0 - 1.0)
#define NUM_LEDS_1 20             // Số LED trên dải 1 (chân mặc định: GPIO 48)
#define NUM_LEDS_2 20             // Số LED trên dải 2 (chân mặc định: GPIO 47)
#include <S4VRobot.h>

S4VRobot Robot;

void setup() {
    Serial.begin(115200);
    Robot.begin("S4VRobot");
    Robot.led_brightness(150);        // Độ sáng ~59%
    Robot.led_set_color(LED_BLUE);    // Khởi động với màu xanh dương
}

void loop() {
    Robot.processInput();

    if (Robot.isAppConnected()) {
        if (GamePad.isCrossPressed())      Robot.arm_downward();
        if (GamePad.isTrianglePressed())   Robot.arm_upward();
        if (GamePad.isSquarePressed())     Robot.grasp();
        if (GamePad.isCirclePressed())     Robot.release();

        if (GamePad.isUpPressed())              Robot.go_forward(100);
        else if (GamePad.isDownPressed())       Robot.go_backward(100);
        else if (GamePad.isLeftPressed())       Robot.turn_left(80);
        else if (GamePad.isRightPressed())      Robot.turn_right(80);
        else                                    Robot.stop();
    } else {
        Robot.stop();
    }

    delay(10);
}
```

## Tài liệu API

### Khởi tạo

| Phương thức | Mô tả |
|--------|-------------|
| `begin(name)` | Khởi tạo BLE, động cơ, servo và LED. `name` là tên thiết bị BLE (mặc định: `"S4VRobot"`). |
| `wait()` | Chờ ~1 giây để Dabble app hoàn tất thiết lập. |
| `wait_app_connection()` | Chặn chương trình cho đến khi ứng dụng Dabble kết nối thành công. |
| `processInput()` | Xử lý dữ liệu BLE đến — gọi mỗi `loop()`. |
| `isAppConnected()` | Trả về `true` nếu ứng dụng Dabble đang kết nối. |

### Điều khiển di chuyển

Tham số `speed` có phạm vi **0 – 100 (%)**.

| Phương thức | Mặc định | Mô tả |
|--------|---------|-------------|
| `go_forward(speed)` | `100` | Di chuyển tiến. |
| `go_backward(speed)` | `100` | Di chuyển lùi. |
| `turn_left(speed)` | `80` | Quay trái (xoay tại chỗ). |
| `turn_right(speed)` | `80` | Quay phải (xoay tại chỗ). |
| `stop()` | — | Dừng cả hai động cơ. |

### Cánh tay & Kẹp

Tham số `step` là bước thay đổi góc servo mỗi lần gọi hàm (`step > 0`; bước càng lớn, cánh tay/kẹp di chuyển càng nhanh).

| Phương thức | Mặc định `step` | Phạm vi servo | Mô tả |
|--------|---------|---------|-------------|
| `arm_upward(step)` | `1.0` | 0° – 180° | Nâng cánh tay lên một bước. |
| `arm_downward(step)` | `1.0` | 0° – 180° | Hạ cánh tay xuống một bước. |
| `grasp(step)` | `1.0` | 0° – 90° | Đóng kẹp một bước. |
| `release(step)` | `1.0` | 0° – 90° | Mở kẹp một bước. |

> **Vị trí mặc định:** Cánh tay khởi động ở 180° (thấp nhất). Kẹp khởi động ở 90° (đóng hoàn toàn).

### Điều khiển LED WS2812

Tham số `strip` chọn dải LED cần điều khiển — mặc định là `LED_STRIP_ALL` (cả 2 dải) hoặc `LED_STRIP_1` tùy hàm.

**Giá trị `LedStrip`:**

| Giá trị | Mô tả |
|---------|-------|
| `LED_STRIP_ALL` | Áp dụng cho cả 2 dải (mặc định của hầu hết hàm) |
| `LED_STRIP_1` | Chỉ dải LED 1 |
| `LED_STRIP_2` | Chỉ dải LED 2 |

**Màu định sẵn `LedColor`:**
`LED_RED` · `LED_GREEN` · `LED_BLUE` · `LED_YELLOW` · `LED_CYAN` · `LED_MAGENTA` · `LED_WHITE` · `LED_OFF`

**Các hàm điều khiển:**

| Phương thức | Mặc định `strip` | Mô tả |
|--------|---------|-------------|
| `led_set_color(color, strip)` | `LED_STRIP_ALL` | Đặt toàn bộ dải sang màu định sẵn. |
| `led_set_color(r, g, b, strip)` | `LED_STRIP_ALL` | Đặt toàn bộ dải sang màu RGB tùy chỉnh. |
| `led_set_pixel(index, color, strip)` | `LED_STRIP_1` | Đặt một LED đơn lẻ sang màu định sẵn. |
| `led_set_pixel(index, r, g, b, strip)` | `LED_STRIP_1` | Đặt một LED đơn lẻ sang màu RGB tùy chỉnh. |
| `led_off(strip)` | `LED_STRIP_ALL` | Tắt dải LED. |
| `led_brightness(brightness, strip)` | `LED_STRIP_ALL` | Điều chỉnh độ sáng (0–255). |

### Nâng cao

| Phương thức | Mô tả |
|--------|-------------|
| `control_motor(motor, direction, speed)` | Điều khiển trực tiếp một động cơ. `motor`: `LEFT_MOTOR` / `RIGHT_MOTOR`; `direction`: `CLOCKWISE` / `COUNTER_CLOCKWISE`; `speed`: 0–255 (raw PWM, có áp dụng soft-speed và offset). |

## Cài đặt ứng dụng Dabble

1. Cài đặt **Dabble** trên điện thoại ([Google Play](https://play.google.com/store/apps/details?id=io.dabbleapp) / [App Store](https://apps.apple.com/app/dabble-diy-arduino-projects/id1472298547)).
2. Mở ứng dụng → chọn module **GamePad**.
3. Kết nối với thiết bị BLE tên `S4VRobot` (hoặc tên tùy chỉnh của bạn).

## Thư viện phụ thuộc

- [DabbleESP32](https://github.com/STEMpedia/DabbleESP32) — đã được tích hợp trong thư viện này
- [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) — đã được tích hợp trong thư viện này
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) — đã được tích hợp trong thư viện này

## Giấy phép

MIT License — xem [LICENSE](LICENSE) để biết chi tiết.

## Tác giả

ThangND — [thang.nd@steamforvietnam.org](mailto:thang.nd@steamforvietnam.org)  
Steam for Vietnam
