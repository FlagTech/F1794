#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#define BITS 10
#define PWM_PIN 15

String pwm_val;

const char* ssid = "Wi-Fi網路名稱";
const char* password = "網路密碼";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  // 設定PWM
  pinMode(PWM_PIN, OUTPUT);
  analogSetAttenuation(ADC_11db);  // 設定類比輸入電壓上限3.6V
  analogSetWidth(BITS);            // 取樣設成10位元
  ledcSetup(0, 5000, BITS);        // 設定PWM，通道0、5KHz、10位元
  ledcAttachPin(PWM_PIN, 0);

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

  server.on("/sw", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->hasParam("led")) {
      AsyncWebParameter* p = request->getParam("led");
      if (p->value() == "on") {
        digitalWrite(LED_BUILTIN, LOW);
      } else if (p->value() == "off") {
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }

    request->send(200, "text/plain", "OK!");
  });

  server.on("/pwm", HTTP_GET, [](AsyncWebServerRequest * req) {
    if (req->hasParam("val")) {
      AsyncWebParameter* p = req->getParam("val");
      pwm_val = p->value();
      ledcWrite(0, pwm_val.toInt());
    }

    req->send(200, "text/plain", "OK");
  });

  server.begin();
  Serial.println("HTTP伺服器開工了～");
}

void loop() { }
