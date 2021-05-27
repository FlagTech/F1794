#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;  // 宣告VL53L0X類型物件

void setup() {
  Serial.begin(115200);
  Wire.begin();  // 啟動I2C通訊

  sensor.setTimeout(500);  // 設定感測器逾時時間
  // 若無法初始化感測器（如：硬體沒有接好），則顯示錯誤訊息。
  if (!sensor.init()) {
    Serial.println("無法初始化VL53L0X感測器～");
    while (1) delay(10);
  }
}

void loop() {
  // 在序列埠監控視窗顯示測距值
  Serial.printf("%umm\n", sensor.readRangeSingleMillimeters());
  // 若發生逾時（感測器沒有回應），則顯示“TIMEOUT”。
  if (sensor.timeoutOccurred()) {
    Serial.println("逾時未回應～");
  }
}
