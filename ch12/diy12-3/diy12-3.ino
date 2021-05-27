//#include <FS.h>
#include <SD.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Wi-Fi名稱";
const char* password = "Wi-Fi密碼";

AsyncWebServer server(80);

void listFile() {
  File root = SD.open("/");    // 以預設（r）模式開啟MicroSD卡根目錄
  File file = root.openNextFile();  // 從根目錄開啟檔案
  while (file) {  // 開啟所有檔案
    Serial.printf("檔名：%s\n", file.name());
    file = root.openNextFile();
  }

  Serial.printf("記憶卡大小：%uMB\n", SD.cardSize() / (1024 * 1024));
  // Serial.printf("記憶卡大小：%uMB\n", SD.cardSize() >> 20);
  Serial.printf("總空間：%uMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("已使用：%uMB\n", SD.usedBytes() / (1024 * 1024));
}

void setup() {
  Serial.begin(115200);

  if (!SD.begin()) {
    Serial.println("無法掛載SD記憶卡～");
    return;
  }

  Serial.print("記憶卡類型：");
  uint8_t cardType = SD.cardType();
  switch (cardType) {
    case CARD_NONE:
      Serial.println("沒有插入SD記憶卡～");
      return;
    case CARD_MMC:
      Serial.println("MMC");  // 記憶卡是MMC類型
      break;
    case CARD_SD:
      Serial.println("SDSC");  // 記憶卡是SDSC類型
      break;
    case CARD_SDHC:
      Serial.println("SDHC");  // 記憶卡是SDHC類型
      break;
    default:
      Serial.println("未知");
  }

  // 連結Wi-Fi相關程式
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP位址：");
  Serial.println(WiFi.localIP());

  server.serveStatic("/", SD, "/www/").setDefaultFile("index.html");
  server.begin();  // 啟動HTTP伺服器
  Serial.println("HTTP伺服器開工了～");
  listFile();   // 列舉檔案
}

void loop() { }
