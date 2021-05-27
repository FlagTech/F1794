#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
int i = 0;    // 計數值
String msg;  // 顯示在螢幕的訊息字串

void setup() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
}

void loop() {
  msg = "Count:" + (String)(++i);
  u8g2.firstPage();
  do {
    u8g2.setCursor(5, 25);
    u8g2.print(msg.c_str());
  }while (u8g2.nextPage());
  delay(500);
}
