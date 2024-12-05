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

#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  ledcAttachChannel(PWM_PIN, 5000, BITS, 0);  // 接腳, 頻率, 解析度, 通道
#else
  ledcSetup(0, 5000, BITS);   // PWM預設為20KHz，10位元解析度。
  ledcAttachPin(PWM_PIN, 0);
#endif

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
      // 新版程式庫要求加上const宣告
      const AsyncWebParameter* p = request->getParam("led");
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
      const AsyncWebParameter* p = req->getParam("val");
      pwm_val = p->value();
    #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
      ledcWrite(PWM_PIN, pwm_val.toInt());
    #else
      ledcWrite(0, pwm_val.toInt());
    #endif
    }

    req->send(200, "text/plain", "OK");
  });

  server.begin();
  Serial.println("HTTP伺服器開工了～");
}

void loop() { }
