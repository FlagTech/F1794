#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "WiFi網路名稱";
const char* password = "網路密碼";

WebServer server(80);  // 建立網站伺服器物件

void handleRoot() {
  String HTML = "<!DOCTYPE html>\
  <html><head><meta charset='utf-8'></head>\
  <body>漫漫長路，總要從第一步開始。\
  </body></html>";
  server.send(200, "text/html", HTML);
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);      // 設定成STA模式
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nIP位址：");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/about", []() {
    server.send(200, "text/html; charset=utf-8", "是在哈囉喔？");
  });
  server.onNotFound([]() {
    server.send(404, "text/plain", "File NOT found!");
  });

  server.begin();
}

void loop(void) {
  server.handleClient();
}
