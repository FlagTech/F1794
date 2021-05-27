#include <U8g2lib.h>
#include "myFont.h"

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
  u8g2.begin();
  u8g2.setFont(myFont);   // 使用自訂的中文字體
  u8g2.enableUTF8Print();  // 啟用顯示UTF-8 編碼字串

  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 16);
    u8g2.print("信心源自日常積累");
  } while ( u8g2.nextPage() );
}

void loop() { }
