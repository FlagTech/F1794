#if CONFIG_FREERTOS_UNICORE     // 確認ESP32的執行核心
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include <WiFi.h>
#include <ESPmDNS.h>
#include <HTTPSServer.hpp>   // HTTPS伺服器程式庫
#include <SSLCert.hpp>       // 處理SSL/TSL憑證
#include <HTTPRequest.hpp>   // 處理HTTP請求
#include <HTTPResponse.hpp>  // 處理HTTP回應
#include <SPIFFS.h>          // 處理SPIFFS檔案系統
#include "cer.h"            // 憑證檔
#include "private_key.h"     // 私鑰檔
#define BITS 10              // PWM輸出解析度（10位元）
#define PWM_PIN 15           // PWM輸出接腳

const char* host = "jarvis";
const char* ssid = "Wi-Fi名稱";
const char* password = "Wi-Fi密碼";

const char contentTypes[][2][32] = {   // MIME類型
  {".txt",  "text/plain"},
  {".html",  "text/html"},
  {".css",  "text/css"},
  {".js",   "application/javascript"},
  {".json", "application/json"},
  {".png",   "image/png"},
  {".jpg",   "image/jpg"},
  {".ico",   "image/x-icon"},
  {"", ""}
};

using namespace httpsserver;

SSLCert cert = SSLCert(
                 example_crt_DER, example_crt_DER_len,
                 example_key_DER, example_key_DER_len
               );

HTTPSServer secureServer = HTTPSServer(&cert);

void handleRoot(HTTPRequest * req, HTTPResponse * res) {
  File homePage = SPIFFS.open("/index.html", "r");
  if (homePage) {
    res->setHeader("Content-Type", "text/html; charset=UTF-8");
    res->println(homePage.readString());
    homePage.close();
  } else {
    req->discardRequestBody();

    res->setStatusCode(500);  // 設定HTTP回應狀態碼（內部伺服器錯誤）
    res->setStatusText("Internal Server Error");

    // Set content type of the response
    res->setHeader("Content-Type", "text/html; charset=UTF-8");
    res->println("<!DOCTYPE html>");
    res->println("<html>");
    res->println("<head><title>伺服器錯誤</title></head>");
    res->println("<body><h1>伺服器出錯啦！</h1><p>網頁被狗狗吃掉了～</p></body>");
    res->println("</html>");
  }
}

void handleFile(HTTPRequest * req, HTTPResponse * res) {
  std::string filename = req->getRequestString();
  Serial.printf("請求資源：%s\n", filename.c_str());
  if (!SPIFFS.exists(filename.c_str())) {  
    req->discardRequestBody();

    res->setStatusCode(404);  // 設定HTTP回應狀態碼（找不到資源）
    res->setStatusText("Not Found");
    res->setHeader("Content-Type", "text/html; charset=UTF-8");
    res->println("<!DOCTYPE html>");
    res->println("<html>");
    res->println("<head><title>找不到</title></head>");
    res->println("<body><h1>404錯誤：找不到檔案！</h1><p>被狗狗吃掉了～</p></body>");
    res->println("</html>");
    return;
  }

  File file = SPIFFS.open(filename.c_str());  // 讀取檔案
  res->setHeader("Content-Length", intToString(file.size()));

  int index = 0;
  do {
    if (filename.rfind(contentTypes[index][0]) != std::string::npos) {
      res->setHeader("Content-Type", contentTypes[index][1]);
      break;
    }
    index ++;
  } while (strlen(contentTypes[index][0]) > 0);

  uint8_t buffer[256];
  size_t length = 0;
  do {
    length = file.read(buffer, 256);
    res->write(buffer, length);
  } while (length > 0);

  file.close();
}

void handlePWM(HTTPRequest * req, HTTPResponse * res) {
  std::string paramName = "val";
  std::string paramVal;
  uint16_t pwmVal = 0;

  ResourceParameters * params = req->getParams();
  if (params->getQueryParameter(paramName, paramVal)) {
    pwmVal = atoi(paramVal.c_str());
    Serial.printf("收到PWM值：%u\n", pwmVal);
    ledcWrite(0, pwmVal);
  }

  res->setHeader("Content-Type", "text/plain");
  res->println("ok");
}

void handleSW(HTTPRequest * req, HTTPResponse * res) {
  ResourceParameters * params = req->getParams();
  std::string swVal;
  if (params->getPathParameter(0, swVal)) {
    Serial.printf("收到led值：%s\n", swVal.c_str());
    if (swVal == "on") {
      digitalWrite(LED_BUILTIN, LOW);
    } else if (swVal == "off") {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }

  res->setHeader("Content-Type", "text/plain");
  res->println("ok");
}

void serverTask(void *params) {
  ResourceNode* nodeRoot = new ResourceNode("/", "GET", handleRoot);
  ResourceNode* nodePWM  = new ResourceNode("/pwm", "GET", handlePWM);
  ResourceNode* nodeSW   = new ResourceNode("/sw/*", "GET", handleSW);
  ResourceNode* nodeFile = new ResourceNode("", "GET", handleFile);

  secureServer.registerNode(nodeRoot);    // 處理首頁請求
  secureServer.registerNode(nodePWM);     // 處理/pwm（亮度調整）請求
  secureServer.registerNode(nodeSW);      // 處理/sw/*（燈光開關）請求
  secureServer.setDefaultNode(nodeFile);  // 處理其他所有路徑的請求

  secureServer.start();  // 啟動伺服器
  if (secureServer.isRunning()) {
    Serial.println("網站伺服器開工了～");

    while (true) {
      secureServer.loop();
      vTaskDelay(1);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);     // 內建的LED腳
  digitalWrite(LED_BUILTIN, HIGH);
  // 設定PWM
  pinMode(PWM_PIN, OUTPUT);
  analogSetAttenuation(ADC_11db);  // 設定類比輸入電壓上限3.6V
  analogSetWidth(BITS);            // 取樣設成10位元
  ledcSetup(0, 5000, BITS);        // 設定PWM，通道0、5KHz、10位元
  ledcAttachPin(PWM_PIN, 0);       // 指定內建的LED接腳成PWM輸出

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nIP位址：");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin(host)) { //http://jarvis.local
    Serial.println("設置MDNS回應器時出錯了～");
    while (1) delay(10);
  }

  if (!SPIFFS.begin(true)) {
    Serial.println("無法掛載SPIFFS檔案系統～");
    while (1) delay(10);
  }

  MDNS.setInstanceName("Cubie's ESP32");
  MDNS.addService("https", "tcp", 443);

  xTaskCreatePinnedToCore(serverTask, "https443", 6144, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
}

void loop() { }
