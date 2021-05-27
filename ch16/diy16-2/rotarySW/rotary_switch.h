#include <switch.h>

class RotarySwitch {
  private:
    byte clkPin;      // CLK接腳
    byte dtPin;       // DT接腳
    bool prev = LOW;  // 前次旋轉狀態
    bool now = LOW;   // 這次旋轉狀態
    Switch* pSW;      // 開關物件

  public:
    typedef enum:byte {
      RELEASED,
      PRESSED,
      LONG_PRESSED,
      PRESSING,
      RELEASED_FROM_PRESS,
      RELEASED_FROM_LONGPRESS,
      TURN_LEFT,
      TURN_RIGHT
    } Status;
    
    Status status = RELEASED;

    RotarySwitch(byte _clk, byte _dt, byte _sw) {
      clkPin = _clk;
      dtPin = _dt;
      pSW = new Switch(_sw, LOW, true);

      pinMode(clkPin, INPUT);
      pinMode(dtPin, INPUT);

      prev = digitalRead(clkPin);
    }

    Status check() {
      Status swState = (Status)pSW->check();
      now = digitalRead(clkPin);

      if (now != prev) {
        if (digitalRead(dtPin) != now) {
          status = TURN_RIGHT;
        } else {
          status = TURN_LEFT;
        }
      } else {
        status = RELEASED;
      }
      prev = now;

      if (swState != RELEASED) {
        status = swState;
      }
      return status;
    }
};
