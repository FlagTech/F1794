#include <BleKeyboardMouse.h>   // 引用藍牙鍵盤滑鼠程式庫
#include <Wire.h>
#include <VL53L0X.h>

unsigned long previousMillis = 0;
const long interval = 10 * 1000;
bool switched = false;

BleKeyboardMouse bleKB("媒體控制按鍵");  // 建立鍵盤滑鼠物件
VL53L0X sensor;     // 宣告VL53L0X類型物件

void setup() {
  Serial.begin(115200);
  Wire.begin();

  sensor.setTimeout(500);  // 設定感測器逾時時間
  if (!sensor.init()) {
    Serial.println("無法初始化VL53L0X感測器～");
    while (1) delay(10);
  }
  bleKB.begin();  // 啟動藍牙鍵盤滑鼠
}

void loop() {
  if (bleKB.isConnected()) {  // 若藍牙已連線…
    if (!switched) {        // 若尚未送出切換桌面訊息…
      int mm = sensor.readRangeSingleMillimeters();

      if (mm < 400) {
        bleKB.press(KEY_LEFT_GUI);   // 按下組合鍵
        bleKB.press(KEY_LEFT_CTRL);
        bleKB.press(KEY_LEFT_ARROW);
        delay(100);
        bleKB.releaseAll();
        switched = true;        // 設成「已切換桌面」
        previousMillis = millis();  // 紀錄切換桌面的時間
        Serial.println("切換到上一個桌面！");
      }
    } else {
      if (millis() - previousMillis > interval) {  // 若超過間隔時間…
        switched = false;       // 設成「尚未切換桌面」
        Serial.println("開始偵測～");
      }
    }
  }
}
