// 自訂常數SLEEP_TIMER_H，避免此程式庫被重複引用。
#ifndef SLEEP_TIMER_H
#define SLEEP_TIMER_H
#include <time.h>

class SleepTimer {
  private:
    uint8_t  tolerance;    // 緩衝秒數
    const char* ntpServer = "pool.ntp.org";

    void goSleep(uint32_t sec) {      // 設定喚醒時間並啟動深度睡眠
      esp_sleep_enable_timer_wakeup(sec * 1000000L);  // 喚醒時間（微秒）
      Serial.printf("喚醒秒數：%u\n", sec);
      Serial.println("睡覺了～");
      delay(50);
      esp_deep_sleep_start();  // 進入深度睡眠
    }

  public:
    SleepTimer (uint8_t sec) {  // 建構式
      tolerance = sec;        // 設定容許誤差秒數
    }

    void init(uint16_t utcOffest = 28800, uint8_t daylightOffset = 0) {
      configTime(utcOffest, daylightOffset, ntpServer);  // 透過網路對時
    }

    int8_t start(uint8_t mn, uint8_t sec, void (*ptFunc)()) {
      if (mn > 60 || mn < 0) return -2;  // 分要介於0~60
      if (sec > 60 || sec < 0) return -3;  // 秒要介於0~60

      struct tm now;
      if (!getLocalTime(&now)) return -1; // 無法取得本機時間

      int8_t mnNow = now.tm_min;  // 此刻的「分」
      int8_t secNow = now.tm_sec;   // 此刻的「秒」
      Serial.printf("設定時間：%d分%d秒\n", mn, sec);
      Serial.printf("現在時間：%d分%d秒\n", mnNow, secNow);

      if (mn == 0) mn = 60;
      if (mnNow == 0) mnNow = 60;

      int16_t diffTime = (mn * 60 + sec) - (mnNow * 60 + secNow); // 比較時間差
      Serial.printf("相差秒數：%d\n", diffTime);

      if (abs(diffTime) <= tolerance) {  // 若時間差小於等於容許誤差…
        (*ptFunc)();               // 執行回呼
        goSleep(diffTime + 3600);    // 進入深度睡眠（+隔1小時的秒數）
      } else {  // 若秒數相差超過容許誤差值…
        if (diffTime < 0) {
          diffTime += 3600;
        }
        goSleep(diffTime);  // 進入深度睡眠
      }

      return 1;
    }
};
#endif
