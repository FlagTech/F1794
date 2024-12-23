#include <switch.h>
#define BITS 10    // 10位元深度
#define STEPS 20   // 設定20階層變化

const byte LED = 22;
const byte UP_SW = 19;     // 調亮開關
const byte DOWN_SW = 18;  // 調暗開關
const byte CHANG_VAL = 1024 / STEPS;  // 每次調光的變化值

Switch upSW(UP_SW, LOW, true);
Switch downSW(DOWN_SW, LOW, true);

int pwmVal = 0;         // 電源輸出值

void lightUp() {
  if ((pwmVal + CHANG_VAL) <= 1023) {
    pwmVal += CHANG_VAL;
    Serial.println(pwmVal);
  #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    ledcWrite(LED, pwmVal);
  #else
    ledcWrite(0, pwmVal);
  #endif
  }
}

void lightDown() {
  if ((pwmVal - CHANG_VAL) >= 0) {
    pwmVal -= CHANG_VAL;
    Serial.println(pwmVal);
  #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    ledcWrite(LED, pwmVal);
  #else
    ledcWrite(0, pwmVal);
  #endif
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  // 設定類比輸出
  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);

#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  ledcAttachChannel(LED, 5000, BITS, 0);  // 接腳, 頻率, 解析度, 通道
#else
  ledcSetup(0, 5000, BITS);   // PWM預設為20KHz，10位元解析度。
  ledcAttachPin(LED, 0);
#endif
}

void loop() {
  switch (upSW.check()) {    // "上"按鍵
    case Switch::RELEASED_FROM_PRESS:
    case Switch::PRESSING:
      lightUp();
      break;
  }

  switch (downSW.check()) {  // "下"按鍵
    case Switch::RELEASED_FROM_PRESS:
    case Switch::PRESSING:
      lightDown();
      break;
  }
}
