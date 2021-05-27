#define BITS 10  // 10位元解析度

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);
}

void loop() {
  uint16_t adc = analogRead(A0);  // 讀取A0腳的類比值
  Serial.printf("MQ-2感測值：%u\n", adc);
  delay(1000);
}
