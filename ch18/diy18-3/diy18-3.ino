#include <U8g2lib.h>
#define BITS 10
#define ADC_RES 1023

const byte thermalPin = A0;  // 熱敏電阻接腳
const byte cdsPin = A4;      // 光敏電阻接腳

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

typedef struct sensorStruct {
  byte pin;
  float value;
} Sensor_t;

QueueHandle_t queue;

// 把熱敏電阻分壓值轉成攝氏溫度
float temperature( uint16_t adc, float T0 = 25.0, uint16_t R0 = 10000, float beta = 3950.0 ) {
  T0 += 273.15;
  float r = (adc * R0) / (ADC_RES - adc);
  return 1 / (1 / T0 + 1 / beta * log(r / R0)) - 273.15;
}

void taskOLED(void * pvParam) {  // 顯示器任務
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  Sensor_t data;
  float cdsVal, tempVal = 0;

  while (1) {
    xQueueReceive(queue, &data, portMAX_DELAY);
    switch (data.pin) {
      case cdsPin:
        cdsVal = (int) data.value;  // 儲存亮度
        break;
      case thermalPin:
        tempVal = data.value;    // 儲存溫度
        break;
    }

    u8g2.firstPage();
    do {
      u8g2.drawStr(0, 10, (String("cds: ") + (int)cdsVal).c_str());
      u8g2.drawStr(0, 25, ("temp: " + String(tempVal, 2)).c_str());
    } while ( u8g2.nextPage() );
  }
}

void taskCds(void * pvParam) {
  int adc = 0;
  Sensor_t cds;
  cds.pin = cdsPin;

  while (1) {
    adc = analogRead(cdsPin);
    cds.value = (float) adc;

    xQueueSend(queue, &cds, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(250));
  }
}

void taskTemp(void *pvParam) {  // 熱敏感測任務
  uint16_t adc = 0;
  Sensor_t thermal;
  thermal.pin = thermalPin;

  while (1) {
    adc = analogRead(thermalPin);    // 取得熱敏感測值
    thermal.value = temperature(adc);

    xQueueSend(queue, &thermal, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(250));
  }
}

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);

  queue = xQueueCreate(10, sizeof(Sensor_t));  // 建立佇列
  if (queue == NULL) {
    Serial.println("無法建立佇列～");
    return;
  }

  xTaskCreate(taskCds, "Cds task", 1500, NULL, 1, NULL );
  xTaskCreate(taskTemp, "thermal task", 1500, NULL, 1, NULL );
  xTaskCreate (taskOLED, "display task", 1500, NULL, 1, NULL);
}

void loop() { }
