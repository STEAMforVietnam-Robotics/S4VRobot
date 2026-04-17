#include <S4VRobot.h>

S4VRobot Robot;

void setup() {
  Serial.begin(115200);
  Robot.begin("S4VRobot");
}

void loop()
{
    Robot.processInput();
    if (GamePad.isCrossPressed())
    {
        Robot.arm_downward();
    }
    if (GamePad.isTrianglePressed())
    {
        Robot.arm_upward();
    }
    if(GamePad.isSquarePressed())
    {
        Robot.grasp();
    }
    if(GamePad.isCirclePressed())
    {
        Robot.release();
    }
    if(GamePad.isUpPressed())
    {
        Robot.go_forward();
    } else if(GamePad.isDownPressed())
    {
        Robot.go_backward();
    }else if(GamePad.isLeftPressed())
    {
        Robot.turn_left();
    }else if(GamePad.isRightPressed())
    {
        Robot.turn_right();
    } else 
    {
        Robot.stop();
    }
    delay(5);
}