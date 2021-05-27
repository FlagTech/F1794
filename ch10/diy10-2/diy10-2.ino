void setup() {
  Serial.begin(115200);   // 連接電腦USB介面的序列埠
  Serial2.begin(9600);    // 連接GPS模組的序列埠
}
void loop() {
  while (Serial2.available() > 0) {  // 若GPS有資料傳入…
    Serial.print(Serial2.read());
  }
}
