#include <U8g2lib.h>
#define LED 5       // LED接腳
#define ISR_PIN 33  // 中斷輸入腳

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
uint8_t score = 0;  // 紀錄成績
volatile bool flashLight = false;  // 是否快閃LED，預設為「否」。
SemaphoreHandle_t xSem;  // 2進位旗號參照

void IRAM_ATTR ISR() {
  flashLight = true;
  xSemaphoreGiveFromISR(xSem, NULL);
}

void taskOLED(void * pvParam) {
  u8g2.begin();
  u8g2.setFont(u8g2_font_inb63_mn);  // 63像素高的數字字體
  String txt;          // OLED顯示文字

  while (1) {
    if (score < 10) {  // 若數字不是雙位數，則前補0。
      txt = "0" + String(score);
    } else {
      txt = String(score);
    }

    u8g2.firstPage();
    do {
      u8g2.drawStr(12, 63, txt.c_str());
    } while ( u8g2.nextPage() );
    
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void taskScore(void * pvParam) {
  uint32_t preMs = 0;  // 紀錄觸發時間

  while (1) {
    if (xSemaphoreTake(xSem, portMAX_DELAY) == pdPASS) {
      // 跟前次時間比對，若小於1000 tick則不執行。
      if (xTaskGetTickCountFromISR() - preMs < 1000)
        continue;

      if (++score == 100) score = 0;
      preMs = xTaskGetTickCountFromISR();  // 紀錄時間
    }
  }
}

void taskBlink(void * pvParam) {
  uint8_t counter = 0;     // 閃爍LED的計數器
  pinMode(LED, OUTPUT);

  while (1) {
    digitalWrite(LED, !digitalRead(LED));

    if (!flashLight) {
      vTaskDelay(pdMS_TO_TICKS(500));
    } else {
      if (++counter == 10) {
        counter = 0;
        flashLight = false;
      }
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ISR_PIN, INPUT_PULLUP);   // 中斷腳設成上拉電阻模式
  attachInterrupt(ISR_PIN, ISR, RISING);  // 附加中斷腳與處理函式
  
  xSem = xSemaphoreCreateBinary();  // 建立2進位號誌物件
  if (xSem == NULL) {
    Serial.println("無法建立「2進位號誌」物件～");
  }

  xTaskCreate(taskScore, "score task", 1000, NULL, 2, NULL);
  xTaskCreate(taskBlink, "blink task", 1000, NULL, 1, NULL);
  xTaskCreate(taskOLED,  "OLED task",  1500, NULL, 1, NULL);
}

void loop() { }
