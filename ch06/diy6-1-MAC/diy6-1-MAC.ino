#include <WiFi.h>
#include <esp_wifi.h>

byte mac[] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xA1}; // 自訂的MAC位址

void setup(){
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);  // 切換到Wi-Fi基站模式
  Serial.print("\n舊MAC位址：");
  Serial.println(WiFi.macAddress());
  // 如果編譯出錯，請把WIFI_IF_STA改成ESP_IF_WIFI_STA
  esp_wifi_set_mac(WIFI_IF_STA, &mac[0]);  // 設定MAC位址
  
  Serial.print("新MAC位址：");
  Serial.println(WiFi.macAddress());
}
 
void loop(){ }
