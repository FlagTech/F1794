#include "rotary_switch.h"  // 引用程式庫

// 建立「旋轉開關」物件（CLK腳, DT腳, SW腳）
RotarySwitch rsw(19, 21, 22);

void setup() {
  Serial.begin(115200);
}

void loop() {
  switch (rsw.check()) {   // 檢查「旋轉開關」的狀態
    case RotarySwitch::RELEASED_FROM_PRESS:
      Serial.println("按一下");
      break;
    case RotarySwitch::PRESSING:
      Serial.println("按著…");
      break;
    case RotarySwitch::LONG_PRESSED:
      Serial.println("長按");
      break;
    case RotarySwitch::TURN_RIGHT:
      Serial.println("往上捲動");
      break;
    case RotarySwitch::TURN_LEFT:
      Serial.println("往下捲動");
      break;
  }
}
