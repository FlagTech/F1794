#include <U8g2lib.h>
#include "bmp.h"

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
  u8g2.begin();
  u8g2.firstPage();
  do {
    u8g2.drawXBMP( 0, 0, 128, 64, logo);
  } while ( u8g2.nextPage() );
  delay(3000);
}

void loop() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_profont12_mr);
    u8g2.setCursor(0,8);
    u8g2.print(F("Taipei"));
    u8g2.setFont(u8g2_font_inr16_mf);
    u8g2.setCursor(60,36);
    u8g2.print(F("21.5\xb0"));
    u8g2.setCursor(60,62);
    u8g2.print(F("34%"));
    // 繪製點陣圖
    u8g2.drawXBMP( 0, 15, 48, 48, _02d);
  } while ( u8g2.nextPage() );
}
