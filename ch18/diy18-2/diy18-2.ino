#include <U8g2lib.h>
#define BITS 10
#define ADC_PIN  A4

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
QueueHandle_t queue;    // 宣告佇列

void taskOLED(void * pvParam) {
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr); // 設定字體
  int num = 0;
  String txt = "Cds Value: ";

  while (1) {
    if (xQueueReceive(queue, &num, portMAX_DELAY) == pdPASS) {
      u8g2.firstPage();
      do {
        u8g2.drawStr(0, 10, (txt + num).c_str());
      } while ( u8g2.nextPage() );

      Serial.printf("收到類比值：%d\n", num);
    } else {
      Serial.println("\n佇列是空的～");
    }
  }
}

void taskCds(void * pvParam) {
  int adc = 0;
  while (1) {
    adc = analogRead(ADC_PIN);  // 把adc值寫入佇列
    if (xQueueSend(queue, &adc, portMAX_DELAY) == pdPASS) {
      Serial.printf("寫入佇列: %d\n", adc);
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);  // 延遲500毫秒
  }
}

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);
  queue = xQueueCreate(5, sizeof(int));

  if (queue == NULL) {
    Serial.println("無法建立佇列～");
    return;
  }

  xTaskCreate(taskCds, "Cds task", 1500, NULL, 1, NULL );
  xTaskCreate(taskOLED, "display task", 1500, NULL, 1, NULL);
}
void loop() { }
