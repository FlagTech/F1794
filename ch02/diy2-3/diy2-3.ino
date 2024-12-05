#define BITS 10

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  analogSetAttenuation(ADC_11db);  // 設定類比輸入電壓上限3.6V
  analogSetWidth(BITS);            // 取樣設成10位元

#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  // 適用於ESP32開發平台3.x版
  ledcAttachChannel(LED_BUILTIN, 5000, BITS, 0);  // 接腳, 頻率, 解析度, 通道
#else
  ledcSetup(0, 5000, BITS);        // 設定PWM，通道0、5KHz、10位元
  ledcAttachPin(LED_BUILTIN, 0);   // 指定內建的LED接腳成PWM輸出
#endif
}

void loop() {
  int val = 1023 - analogRead(A4);  // 讓A4類比值大小相反
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  ledcWrite(LED_BUILTIN, val);
#else
  ledcWrite(0, val);
#endif
}
