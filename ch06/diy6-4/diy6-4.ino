#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "WiFi網路名稱";
const char* password = "網路密碼";

WebServer server(80);

void handleRoot() {
  String HTML = "<!DOCTYPE html>\
  <html><head><meta charset='utf-8'></head>\
  <body>沒錯！這是ESP32網站！\
  </body></html>";
  server.send(200, "text/html", HTML);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nIP位址：");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/about", []() {
    server.send(200, "text/html; charset=utf-8", "是在哈囉喔？");
  });
  server.on("/sw", []() {
    String state = server.arg("led");
    if (state == "on") {
      digitalWrite(LED_BUILTIN, LOW);
    } else if (state == "off") {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    server.send(200, "text/html", "LED is <b>" + state + "</b>.");
  });
  server.onNotFound([]() {
    server.send(404, "text/plain", "File NOT found!");
  });
  server.begin();
  Serial.println("網站伺服器開工了～");
}

void loop() {
  server.handleClient();
}
