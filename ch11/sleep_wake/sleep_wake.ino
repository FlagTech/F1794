// 參閱11-4頁

#define WAKEUP_MS 5 * 1000000 // 每5秒喚醒

RTC_DATA_ATTR int wakes = 0; // 紀錄喚醒次數的整數值

void setup()
{
  Serial.begin(115200);
  Serial.printf("喚醒次數：%d\n", wakes);
  wakes++;
  delay(1000);
  Serial.println("進入睡眠 zzz");
  delay(1000);
  esp_sleep_enable_timer_wakeup(WAKEUP_MS);
  esp_deep_sleep_start();
}

void loop() {}
