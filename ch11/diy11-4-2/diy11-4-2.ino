// 參閱11-30頁

RTC_DATA_ATTR int wakes = 0; // 紀錄喚醒次數的整數值

void setup()
{
  Serial.begin(115200);
  Serial.printf("喚醒次數：%d\n", wakes);
  wakes++;
  // 設定讓腳32輸入高電位時喚醒ESP32
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 1);
  Serial.println("進入睡眠 zzz");
  delay(50);
  esp_deep_sleep_start();
}

void loop() {}
