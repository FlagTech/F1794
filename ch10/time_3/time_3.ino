// 參閱10-9頁

#include <time.h>
#include <sys/time.h>
void setup()
{
  Serial.begin(115200);
  time_t today = time(NULL);
  struct tm xmas = {0, 0, 0, 25, 11, 125}; // 2025年12月25日
  today = mktime(&xmas);
  struct timeval tv = {.tv_sec = today};
  struct timezone tz = {.tz_minuteswest = 8 * 60};
  settimeofday(&tv, &tz);
  setenv("TZ", "CST-8", 1); // 「時區」環境變數設成「亞洲/台北」
}

void loop()
{
  delay(1000);
  time_t now = time(NULL);
  printf("ctime的輸出：%s", ctime(&now));
  struct tm *localtm = localtime(&now);
  Serial.println(localtm, "自訂格式：%Y/%m/%d %H:%M:%S");
}
