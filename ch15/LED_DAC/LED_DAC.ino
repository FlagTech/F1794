void setup() { }

void loop() {
  static int deg=0;
  dacWrite(25, int(128+62*sin(deg*PI/180)));
  deg++;
  if (deg>360) deg = 0;
  delay(20);  // 設定輸出訊號的週期時間：20ms × 360
}
