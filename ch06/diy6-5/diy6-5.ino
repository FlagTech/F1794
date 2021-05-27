#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#define LDR_PIN 36

const char* ssid = "Wi-Fi網路名稱";
const char* password = "網路密碼";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("掛載SPIFFS分區出錯啦～");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nIP位址：");
  Serial.println(WiFi.localIP());

  server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
  server.serveStatic("/img", SPIFFS, "/www/img/");
  server.serveStatic("/favicon.ico", SPIFFS, "/www/favicon.ico");

  server.on("/LDR", HTTP_GET, [](AsyncWebServerRequest * req) {
    uint16_t val = analogRead(LDR_PIN);  // 讀取光敏電阻的數值
    req->send(200, "text/plain", String(val));
  });

  server.begin();
  Serial.println("HTTP伺服器開工了～");
}

void loop() { }
