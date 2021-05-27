#define BITS 10

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  analogSetAttenuation(ADC_11db);  // 設定類比輸入電壓上限3.6V
  analogSetWidth(BITS);            // 取樣設成10位元
  ledcSetup(0, 5000, BITS);        // 設定PWM，通道0、5KHz、10位元
  ledcAttachPin(LED_BUILTIN, 0);   // 指定內建的LED接腳成PWM輸出
}

void loop() {
  int val = 1023 - analogRead(A4);  // 讓A4類比值大小相反
  ledcWrite(0, val);
}
