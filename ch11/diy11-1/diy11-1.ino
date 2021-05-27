#define THRESHOLD 40
RTC_DATA_ATTR int wakes = 0;
const byte touchPin = T3;   // 指定觸控腳

void callback(){
  // 此例沒用到中斷回呼函式，所以這裡保持空白。
}

void setup(){
  Serial.begin(115200);
  Serial.printf("喚醒次數：%d\n", wakes);
  wakes ++;
  delay(1000);
  Serial.println("進入睡眠 zzz");
  delay(1000);
  touchAttachInterrupt(touchPin, callback, THRESHOLD);
  esp_sleep_enable_touchpad_wakeup();
  esp_deep_sleep_start();
}

void loop(){ }
