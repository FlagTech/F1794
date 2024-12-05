#include <WiFi.h>
#include <SPIFFS.h>  // 引用操作SPIFFS的程式庫
#include <WiFiClientSecure.h>
#define CA_FILE "/thingspeak.cer"    // 憑證檔名   
#define CHANNEL_ID "你的頻道ID"      // ThingSpeak頻道ID
#define API_KEY "頻道的Read API碼"   // ThingSpeak READ API碼

const char* ssid = "Wi-Fi網路的名稱";
const char* password = "Wi-Fi密碼";
const char* SERVER = "api.thingspeak.com";  // 伺服器網域
WiFiClientSecure client;   // 宣告HTTPS前端物件
String root_ca;            // 儲存CA憑證的變數

bool readFile(String path) {   // 讀取檔案
  File file = SPIFFS.open(path, "r");  // 以「唯讀模式」開啟
  if (!file) {
    Serial.println("無法開啟檔案～");
    return 0;
  }

  root_ca = file.readString();
  file.close();
  return 1;
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  if (!SPIFFS.begin(true)) {
    Serial.println("無法掛載SPIFFS檔案系統～");
    while(1) delay(10);  // 程式將停在這裡…
  }
  
  if (readFile(CA_FILE)) {  // 讀取憑證檔，若讀取成功…
    client.setCACert(root_ca.c_str());  // 設定憑證
  } else {               // 否則…
    while(1) delay(10);  // 程式將停在這裡…
  }

  Serial.println("\n開始連接伺服器…");
  if (!client.connect(SERVER, 443)) {
    Serial.println("連線失敗～");
  } else {
    Serial.println("連線成功！");
    String https_get = "GET https://" + String(SERVER) +\
                       "/channels/" + CHANNEL_ID +\
                       "/feeds.json?api_key=" + API_KEY +\
                       "&results=1 HTTP/1.1\n"\
                       "Host: " + String(SERVER) + "\n" +\
                       "Connection: close\n\n";
                       
    client.print(https_get);

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("收到HTTPS回應：");
        break;
      }
    }
    // 接收並顯示伺服器的回應
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
    client.stop();
  }
}

void loop() { }
