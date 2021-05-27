#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <Update.h>

const char *host = "jarvis"; // 自訂的本機域名
const char *ssid = "Wi-Fi網路名稱";
const char *password = "Wi-Fi密碼";

AsyncWebServer server(80); // 建立網站伺服器物件

void handleUpdate(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    Serial.printf("更新韌體：%s\n", filename.c_str());
    if (!Update.begin())
    {
      Update.printError(Serial);
    }
  }

  if (len)
  {
    Update.write(data, len);
  }
  if (final)
  {
    if (Update.end(true))
    {
      Serial.printf("成功寫入%u位元組。\n重新啟動ESP32～\n", index + len);
    }
    else
    {
      Update.printError(Serial);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin(true))
  {
    Serial.println("無法掛載SPIFFS");
    while (1)
      delay(50);
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

  if (!MDNS.begin(host))
  { // 啟用jarvis.local域名
    Serial.println("設置MDNS回應器時出錯了～");
    while (1)
      delay(50);
  }

  server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
  server.serveStatic("/firmware", SPIFFS, "/www/firmware.html");
  server.serveStatic("/img", SPIFFS, "/www/img/");
  server.serveStatic("/favicon.ico", SPIFFS, "/www/favicon.ico");

  server.on(
      "/upload", HTTP_POST, [](AsyncWebServerRequest *req) {
        req->send(200, "text/html; charset=utf-8",
                  (Update.hasError()) ? "更新失敗～" : "更新成功！");
        delay(3000);
        ESP.restart(); // 重新啟動ESP32
      },
      handleUpdate);

  server.begin();
  MDNS.setInstanceName("Cubie's ESP32");
  MDNS.addService("http", "tcp", 80);
}

void loop() {}
