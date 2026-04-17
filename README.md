# S4VRobot

Arduino library for controlling the S4V Robot car with robotic arm via Bluetooth using the [Dabble](https://thestempedia.com/product/dabble/) smartphone app on ESP32.

## Features

- Bluetooth Low Energy (BLE) communication via Dabble app
- DC motor control (forward, backward, left, right, stop)
- Robotic arm servo control (up/down)
- Gripper servo control (grasp/release)
- GamePad module integration for smartphone-based control

## Hardware Requirements

| Component | Details |
|-----------|---------|
| Microcontroller | ESP32 |
| Motor driver | L298N |
| Drive motors | 2x DC motors (left & right) |
| Arm servo | Connected to pin 17 |
| Gripper servo | Connected to pin 18 |
| App | Dabble (iOS / Android) |

## Pin Mapping

| Pin | Function |
|-----|---------|
| GPIO 4 | L298N ENA (Left motor PWM) |
| GPIO 5 | L298N IN1 |
| GPIO 6 | L298N IN2 |
| GPIO 7 | L298N IN3 |
| GPIO 15 | L298N IN4 |
| GPIO 16 | L298N ENB (Right motor PWM) |
| GPIO 17 | Arm servo signal |
| GPIO 18 | Gripper servo signal |

## Installation

1. Download this repository as a `.zip` file.
2. In Arduino IDE: **Sketch → Include Library → Add .ZIP Library...**
3. Select the downloaded file.
4. Install the **Dabble** library from the Arduino Library Manager as well.

## Quick Start

```cpp
#include <S4VRobot.h>

S4VRobot Robot;

void setup() {
  Serial.begin(115200);
  Robot.begin("S4VRobot"); // BLE device name visible in Dabble
}

void loop() {
  Robot.processInput(); // Must be called every loop

  if (GamePad.isUpPressed())         Robot.go_forward();
  else if (GamePad.isDownPressed())  Robot.go_backward();
  else if (GamePad.isLeftPressed())  Robot.turn_left();
  else if (GamePad.isRightPressed()) Robot.turn_right();
  else                               Robot.stop();

  if (GamePad.isTrianglePressed())   Robot.arm_upward();
  if (GamePad.isCrossPressed())      Robot.arm_downward();
  if (GamePad.isSquarePressed())     Robot.grasp();
  if (GamePad.isCirclePressed())     Robot.release();

  delay(5);
}
```

## API Reference

### Initialization

| Method | Description |
|--------|-------------|
| `begin(name)` | Initialize BLE, motors, and servos. `name` is the BLE device name (default: `"S4VRobot"`). |
| `wait_app_connection()` | Block until the Dabble app connects. |
| `processInput()` | Process incoming BLE data — call every `loop()`. |

### Drive Control

| Method | Description |
|--------|-------------|
| `go_forward(speed)` | Move forward. Default speed: `150` (0–255). |
| `go_backward(speed)` | Move backward. |
| `turn_left(speed)` | Rotate left (pivot turn). |
| `turn_right(speed)` | Rotate right (pivot turn). |
| `stop()` | Stop both drive motors. |

### Arm & Gripper

| Method | Description |
|--------|-------------|
| `arm_upward()` | Raise arm by one step (0–180°). |
| `arm_downward()` | Lower arm by one step. |
| `grasp()` | Close gripper by one step. |
| `release()` | Open gripper by one step. |

## Dabble App Setup

1. Install **Dabble** on your smartphone ([Google Play](https://play.google.com/store/apps/details?id=io.dabbleapp) / [App Store](https://apps.apple.com/app/dabble-diy-arduino-projects/id1472298547)).
2. Open the app → select **GamePad** module.
3. Connect to the BLE device named `S4VRobot` (or your custom name).

## Dependencies

- [DabbleESP32](https://github.com/STEMpedia/DabbleESP32) — included in this library
- [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) — included in this library

## License

MIT License — see [LICENSE](LICENSE) for details.

## Author

ThangND — [thang.nd@steamforvietnam.org](mailto:thang.nd@steamforvietnam.org)  
Steam for Vietnam
