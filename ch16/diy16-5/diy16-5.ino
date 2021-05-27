#include <BleKeyboardMouse.h>
#include <Keypad.h>
#include "rotary_switch.h"

BleKeyboardMouse bleKB("媒體控制按鍵");
// 建立「旋轉開關」物件（CLK腳, DT腳, SW腳）
RotarySwitch rsw(19, 21, 22);

const byte ROWS = 4; // 4列
const byte COLS = 4; // 4行
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {23, 18, 5, 17};
byte colPins[COLS] = {16, 4, 0, 2};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(115200);
  bleKB.begin();       // 啟動藍牙鍵盤滑鼠
}

void loop() {
  if (bleKB.isConnected()) {      // 若與主機連線…
    char key = keypad.getKey();  // 讀取按鍵鍵碼

    switch (key) {
      case '1':
        bleKB.print("love DIY!");   // 在鍵盤上敲擊按鍵
        bleKB.write(KEY_RETURN); // 下Enter/Return鍵
        break;
      case 'C':
        bleKB.press(KEY_LEFT_CTRL); // 按著左Ctrl鍵
        bleKB.press('c');           // 按著小寫c鍵
        delay(100);
        bleKB.releaseAll();         // 0.1秒之後放開所有按鍵
        break;
      case 'D':
        bleKB.press(KEY_LEFT_CTRL);  // 按著左Ctrl鍵
        bleKB.press('v');          // 按著小寫v鍵
        delay(100);
        bleKB.releaseAll();         // 0.1秒之後放開所有按鍵
        break;
      case '*':
        bleKB.write(KEY_MEDIA_VOLUME_DOWN); // 降低音量
        break;
      case '#':
        bleKB.write(KEY_MEDIA_VOLUME_UP);    // 提高音量
        break;
    }
    
    switch (rsw.check()) {   // 檢查「旋轉開關」的狀態…
      case RotarySwitch::RELEASED_FROM_PRESS:  // 開關被「按一下」
        bleKB.click();     // 點擊滑鼠左鍵
        break;
      case RotarySwitch::TURN_RIGHT:      // 旋鈕右轉
        bleKB.move(0,0,-1);   // 往上滑動滾輪
        break;
      case RotarySwitch::TURN_LEFT:       // 旋鈕左轉
        bleKB.move(0,0,1);    // 往下滑動滾輪
        break;
    }
  }
}
