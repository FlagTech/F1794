#include <U8g2lib.h>

//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 32, 33, U8X8_PIN_NONE);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
int i = 0;    // 計數值
String msg;  // 顯示在螢幕的訊息字串

void setup() {
 u8g2.begin();
 u8g2.setFont(u8g2_font_ncenB08_tr);
}

void loop() {
  msg = "Count: " + (String)(++i);
  u8g2.clearBuffer();
  u8g2.drawStr(5, 10, msg.c_str());
  u8g2.sendBuffer();
  delay(1000);
}
