#include "qrcode.h"       // 引用qrcode.h程式庫

void setup() {
  Serial.begin(115200);
  uint32_t dt = millis();   // 紀錄程式起始時間

  QRCode qrcode;       // 建立QR碼物件
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0, "hello");

  dt = millis() - dt;
  Serial.printf("產生QR碼所花費的時間：%dms\n\n", dt);

  for (byte y = 0; y < qrcode.size; y++) {
    for (byte x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {
        Serial.print("\u25A0\u25A0");
      } else {
        Serial.print("\u25A1\u25A1");
      }
    }
    Serial.print("\n");
  }

  Serial.print("\n\n\n\n");
  Serial.print("\n\n\n\n");
}

void loop() {

}
