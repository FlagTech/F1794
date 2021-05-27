// 參閱2-7頁
void setup()
{
  // Serial.begin(115200);
  char *sensor = "相對溼度";
  float val = 32.75;

  printf("%s值為%.2f%%\n", sensor, val);
  // Serial.printf("%s值為%.2f%%\n", sensor, val);
}

void loop() {}
