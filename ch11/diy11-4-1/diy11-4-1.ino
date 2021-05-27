// 參閱11-29頁

#define BITMASK 0x300000000 // 腳32和33的位元遮罩

RTC_DATA_ATTR int wakes = 0; // 紀錄喚醒次數的整數值

void setup()
{
  Serial.begin(115200);
  Serial.printf("喚醒次數：%d\n", wakes);
  wakes++;
  esp_sleep_enable_ext1_wakeup(BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
  Serial.println("進入睡眠 zzz");
  delay(50);
  esp_deep_sleep_start();
}

void loop() {}
