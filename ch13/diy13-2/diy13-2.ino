#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <U8g2lib.h>
#include "qrcode.h"

const char* ssid = "Wi-Fi名稱";
const char* password = "Wi-Fi密碼";

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
QRCode qrcode;
AsyncWebServer server(80);

const char* homePage = "<!DOCTYPE html>\
  <html><head><meta charset='utf-8'>\
  <meta name='viewport' content='width=device-width, initial-scale=1'>\
  </head><body>沒錯！這是ESP32網站！\
  </body></html>";

void drawQRcode(IPAddress ip) {
  byte x0 = 3 + 64;
  byte y0 = 3;
  String url = "http://" + ip.toString();
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3 , 0, url.c_str()); // 產生QR碼
  String subIP;

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_VCR_OSD_tn);
    u8g2.setColorIndex(1);      // 設成白色

    for (byte i = 0; i < 3; i++) {
      subIP = String(ip[i]) + ".";
      u8g2.drawUTF8(0, 16 * (i + 1), subIP.c_str());
    }
    subIP = String(ip[3]);
    u8g2.drawUTF8(0, 64, subIP.c_str());

    u8g2.drawBox(64, 0, 64, 64);
    for (uint8_t y = 0; y < qrcode.size; y ++) {
      for (uint8_t x = 0; x < qrcode.size; x ++) {
        if (qrcode_getModule(&qrcode, x, y)) {
          u8g2.setColorIndex(0);
        } else {
          u8g2.setColorIndex(1);
        }
        u8g2.drawBox(x0 + x * 2, y0 + y * 2, 2, 2);
      }
    }
  } while (u8g2.nextPage());
}


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("IP位址：");
  Serial.println(WiFi.localIP());  // 顯示IP位址

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * req) {
    req->send(200, "text/html", homePage);
  });

  server.on("/sensors.json", HTTP_GET, [](AsyncWebServerRequest * req) {
    int val1 = 321;
    float val2 = 45.67;
    String jsonStr = "{\"sens1\":" + String(val1) +
                     ", \"sens2\":" + String(val2) + "}";
    req->send(200, "application/json", jsonStr);
  });

  server.begin();
  u8g2.begin();
  drawQRcode(WiFi.localIP());
}

void loop() {

}
