#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

TaskHandle_t handler;

void taskOLED( void * pvParam ) {
  u8g2.setFont(u8g2_font_ncenB08_tr);
  int num = 0;    // 計數值
  String txt;     // OLED顯示文字

  while (1) {
    txt = "counter: " + String(num++);
    u8g2.firstPage();
    do {
      u8g2.drawStr(0, 15, txt.c_str());
    } while ( u8g2.nextPage() );
    Serial.printf("顯示器任務在核心%u\n", xPortGetCoreID());
    vTaskDelay( 1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  u8g2.begin();
  Serial.begin(115200);
  Serial.printf("上限：%u\n", configMAX_PRIORITIES);
  xTaskCreatePinnedToCore(taskOLED,  "task OLED", 1500, NULL,  1, NULL, 1);
}

void loop() {
  Serial.printf("主程式在核心%u\n", xPortGetCoreID());
  UBaseType_t m = 1500 - uxTaskGetStackHighWaterMark(handler);
  Serial.printf("任務A佔用 %u位元組\n", m);
  vTaskDelay(1000);
}
