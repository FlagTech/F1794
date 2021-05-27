// 參閱10-4頁

#include <time.h>

time_t t = time(NULL);
clock_t c = clock();
unsigned long ms = millis();

void setup()
{
  Serial.begin(115200);
  time_t today = time(NULL);
  struct tm xmas = {0, 0, 0, 25, 11, (2025 - 1900)};

  /*
   struct tm xmas = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_mday = 25,
    .tm_mon = 11,
    .tm_year = 125
   };
   */

  /*
    struct tm xmas;
    xmas.tm_year = 125;
    xmas.tm_mon = 11;
    xmas.tm_mday = 25;
    */

  printf("設定tm之前：%u\n", today);
  today = mktime(&xmas);
  printf("設定tm之後：%u\n", today);
}
void loop() {}
