// 參閱3-20頁

#include "switch.h"

const byte LED1 = 22;
const byte LED2 = LED_BUILTIN; // 內建的LED
const byte SW_PIN = 23;        // 開關接腳

boolean LED1State = false;
boolean LED2State = false;

Switch sw(SW_PIN, LOW, true);

void setup()
{
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop()
{
  switch (sw.check())
  {
  case RELEASED_FROM_PRESS:
    LED2State = !LED2State;
    digitalWrite(LED2, LED2State);
    break;
  case LONG_PRESSED:
    LED1State = !LED1State;
    digitalWrite(LED1, LED1State);
    break;
  case PRESSING:
    Serial.println("hello!");
    break;
  }
}
