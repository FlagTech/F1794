#include <BluetoothSerial.h>

BluetoothSerial BT; 

char* pin = "9420";
char  val;

void setup(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  BT.setPin(pin);
  BT.begin("ESP32經典好朋友");

  byte macBT[6];
  esp_read_mac(macBT, ESP_MAC_BT);
  Serial.printf("藍牙MAC位址：%02X:%02X:%02X:%02X:%02X:%02X\n", 
macBT[0], macBT[1], macBT[2], macBT[3], macBT[4], macBT[5]);
}

void loop(){
  if (BT.available()){  // 若接收到藍牙序列資料
    val = BT.read();
    switch (val){
    case '0':                       // 若接收到'0'...
      digitalWrite(LED_BUILTIN, LOW); // 關閉LED
      BT.println("關燈～");          // 送出訊息
      break;
    case '1':                       // 若接收到'1'...
      digitalWrite(LED_BUILTIN, HIGH); // 點亮LED
      BT.println("開燈！");
      break;
    }
  }
}
