// 參閱12-13頁

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

const char *ssid = "Wi-Fi名稱";
const char *password = "Wi-Fi密碼";

AsyncWebServer server(80);

void listFile()
{ // 列舉全部檔案
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while (file)
  {
    Serial.println(String("檔名：") + file.name());
    file = root.openNextFile();
  }
}

void handleUpload(AsyncWebServerRequest *req, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    Serial.printf("開始上傳：%s\n", filename);
    req->_tempFile = SPIFFS.open("/www/data/" + filename, "w");
  }

  if (len)
  {
    req->_tempFile.write(data, len); // 寫入上傳檔
  }

  if (final)
  {
    Serial.printf("已上傳：%s、檔案大小：%u\n", filename, (index + len));
    req->_tempFile.close();

    listFile();
  }
}

void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin())
  {
    Serial.println("無法掛載SPIFFS");
    while (1)
    {
      delay(50);
    }
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("IP位址：");
  Serial.println(WiFi.localIP()); // 顯示IP位址

  server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
  server.serveStatic("/firmware", SPIFFS, "/www/").setDefaultFile("firmware.html");
  server.serveStatic("/img", SPIFFS, "/www/img/");
  server.serveStatic("/favicon.ico", SPIFFS, "/www/favicon.ico");

  server.on(
      "/upload", HTTP_POST, [](AsyncWebServerRequest *req) {
        req->send(200, "text/html; charset=utf-8", "<a href='/'>回首頁</a>");
      },
      handleUpload);

  server.begin(); // 啟動網站伺服器
  Serial.println("HTTP伺服器開工了～");
}

void loop() {}
