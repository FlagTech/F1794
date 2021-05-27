// 參閱10-24頁

#include <TinyGPS++.h>
#include <time.h>
#include <sys/time.h>
#define GPS_BAUD 9600

TinyGPSPlus gps;
bool timeSetted = false;

void parseGPS()
{
  uint16_t y = gps.date.year();

  if (y != 2000)
  {
    uint8_t m = gps.date.month();
    uint8_t d = gps.date.day();
    uint8_t hr = gps.time.hour();
    uint8_t mn = gps.time.minute();
    uint8_t sec = gps.time.second();

    time_t today = time(NULL);
    // 時間結構體 {秒, 分, 時, 日, 月（從0開始）, 年（自1900開始）}
    struct tm now = {sec, mn, hr, d, m - 1, (y - 1900)};
    today = mktime(&now);
    struct timeval tv = {.tv_sec = today};
    struct timezone tz = {.tz_minuteswest = 8 * 60};
    settimeofday(&tv, &tz);
    timeSetted = true;
  }
  else
  {
    Serial.printf("GPS日期資料讀取中…\n");
  }
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(GPS_BAUD);
}

void loop()
{
  if (!timeSetted)
  { // 若尚未設定系統RTC時間…
    while (Serial2.available() > 0)
    {
      if (gps.encode(Serial2.read()))
      {
        parseGPS();
      }
    }
  }
  else
  {
    setenv("TZ", "CST-8", 1); // 設成台北時區
    time_t now = time(NULL);
    struct tm *localtm = localtime(&now);
    Serial.println(localtm, "系統時間：%Y/%m/%d %H:%M:%S");
  }
  delay(1000);
}
