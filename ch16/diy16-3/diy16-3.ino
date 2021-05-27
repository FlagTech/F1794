#include <BleKeyboardMouse.h>
#include "rotary_switch.h"

BleKeyboardMouse bleKB("媒體控制旋鈕");
RotarySwitch rsw(19, 21, 22);

void setup() {
  Serial.begin(115200); bleKB.begin(); // 啟動藍牙鍵盤滑鼠物件
}

void loop() {
  if (bleKB.isConnected()) {
    switch (rsw.check()) {
      case RotarySwitch::RELEASED_FROM_PRESS:
        bleKB.write(KEY_MEDIA_PLAY_PAUSE);
        break;
      case RotarySwitch::TURN_RIGHT:
        bleKB.write(KEY_MEDIA_VOLUME_UP);
        break;
      case RotarySwitch::TURN_LEFT:
        bleKB.write(KEY_MEDIA_VOLUME_DOWN);
        break;
    }
  }
}
