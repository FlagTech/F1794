#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

const char* host = "jarvis";
const char* ssid = "Wi-Fi名稱";
const char* password = "Wi-Fi密碼";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("IP位址:");
  Serial.println(WiFi.localIP());  // 顯示IP位址

  if (!MDNS.begin(host)) { //http://jarvis.local
    Serial.println("設置MDNS回應器時出錯了～");
    while (1) {
      delay(1000);
    }
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * req) {
    req->send(200, "text/html; charset=utf-8", "我是ESP32！");
  });

  server.begin();
  MDNS.setInstanceName("Cubie's ESP32");  // 設置實體名稱
  MDNS.addService("http", "tcp", 80);       // 設置服務描述
}

void loop() { }
