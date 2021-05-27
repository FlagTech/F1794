#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void taskOLED( void * pvParam ) {
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  int num = 0;
  String txt; 

  while (1) {
    txt = "Counter: " + String(num++);
    u8g2.firstPage();
    do {
      u8g2.drawStr(0, 15, txt.c_str());  // 在(0, 15)座標顯示文字
    } while ( u8g2.nextPage() );
    vTaskDelay( 1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  xTaskCreate(taskOLED, "task OLED", 1500, NULL, 1, NULL);
}

void loop() { }
