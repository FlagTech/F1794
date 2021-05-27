// 參閱10-2頁

typedef struct data
{ // 定義結構
  byte pin;
  int ms;
} LED;

void setup()
{
  Serial.begin(115200);
  LED led;
  LED *pLed = &led;

  (*pLed).pin = 7;
  (*pLed).ms = 250;

  Serial.printf("pin: %d\n", pLed->pin);
  Serial.printf("ms: %d\n", pLed->ms);
}

void loop() {}
