// 參閱5-17頁

#include <U8g2lib.h>
#include "myFont.h"

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup(void)
{
  u8g2.begin();
  u8g2.setFont(myFont);
}

void loop(void)
{
  u8g2.firstPage();
  do
  {
    u8g2.drawStr(0, 18, "ABC123"); // 顯示“ABC123”
  } while (u8g2.nextPage());
}
