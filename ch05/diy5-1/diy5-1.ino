#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup(void) {
 u8g2.begin();
 u8g2.setFont(u8g2_font_ncenB08_tr);
}

void loop(void) {
  u8g2.clearBuffer();
  u8g2.drawStr(5, 10, "No Hack,");
  u8g2.drawStr(20, 30, "No LIFE!");
  u8g2.sendBuffer();
}
