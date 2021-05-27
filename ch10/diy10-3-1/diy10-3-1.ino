// 參閱10-22頁

#include <TinyGPS++.h>
#define GPS_BAUD 9600 // GPS的序列通訊速率

TinyGPSPlus gps; // 宣告gps物件

void parseGPS()
{
  if (gps.location.isValid())
  {
    float lat = gps.location.lat(); // 緯度
    float lng = gps.location.lng(); // 經度

    Serial.printf("座標：%.6f, %.6f\n", lat, lng);
  }
  else
  {
    Serial.printf("座標資料錯誤\n");
  }

  if (gps.date.isValid())
  {
    uint16_t y = gps.date.year(); // 年
    uint8_t m = gps.date.month(); // 月
    uint8_t d = gps.date.day();   // 日

    Serial.printf("%d/%d/%d ", y, m, d);
  }
  else
  {
    Serial.printf("日期資料錯誤");
  }

  if (gps.time.isValid())
  {
    uint8_t hr = gps.time.hour();    // 時
    uint8_t mn = gps.time.minute();  // 分
    uint8_t sec = gps.time.second(); // 秒

    Serial.printf("%d:%d:%d\n", hr, mn, sec);
  }
  else
  {
    Serial.println("時間資料錯誤");
  }
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(GPS_BAUD);
}

void loop()
{
  while (Serial2.available() > 0)
  {
    if (gps.encode(Serial2.read()))
    {
      parseGPS();
    }
  }
}
