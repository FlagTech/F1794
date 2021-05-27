#include <BluetoothSerial.h>

BluetoothSerial BT;

// 藍牙從端的MAC位址（請自行修改）
byte clientAddr[6]  = {0x24, 0x0A, 0xC4, 0x02, 0xE8, 0xA2};
// 藍牙從端的名稱
String clientName = "ESP32經典好朋友";
bool connected;

void setup() {
  Serial.begin(115200);
  BT.begin("ESP32經典大頭目", true);

  Serial.println("準備藍牙連線…");
  connected = BT.connect(clientAddr);
  // 透過名稱連線
  //connected = BT.connect(clientName);
  
  if(connected) {
    Serial.println("跟好朋友連線了！");
  } else {
    while(!BT.connected(10000)) {
      Serial.println("無法連線，請確認好朋友在旁邊…還要接電喔！"); 
    }
  }
}

void loop() {
  if (Serial.available()) {
    BT.write(Serial.read());
  }
  if (BT.available()) {
    Serial.write(BT.read()); 
  }
}
