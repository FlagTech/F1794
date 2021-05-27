#include <WiFi.h>
#include <time.h>

const char* ssid = "無線網路ID";
const char* password = "網路密碼";

const char* ntpServer = "pool.ntp.org";
const uint16_t utcOffest = 28800;  // UTC+8偏移量
const uint8_t daylightOffset = 0;   // 夏令時間

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
  Serial.println(WiFi.localIP());
  
  configTime(utcOffest, daylightOffset, ntpServer); 
  delay(1000);
}

void loop() {
  struct tm now;                  // 宣告「分解時間」結構變數
  if(!getLocalTime(&now)){          // 取得本地時間
    Serial.println("無法取得時間～");
    return;
  }
  Serial.println(&now, "%Y/%m/%d %H:%M:%S"); // 輸出格式化時間字串
 delay(1000);
}
