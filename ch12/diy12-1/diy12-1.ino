#include <SPIFFS.h>

void writeFile() {    // 寫入檔案
  File file = SPIFFS.open("/test.txt", "w");
  if (!file) {
    Serial.println("無法開啟檔案～");
    return;
  } 
  file.println("因為難，所以好玩！");
  file.close();
}

void readFile() {   // 讀取檔案
  File file = SPIFFS.open("/test.txt", "r");
  if (!file) {
    Serial.println("無法開啟檔案～");
    return;
  }
  /*
   * 
  while (file.available()) {
    Serial.print(file.readStringUntil('\n'));  // 持續讀取到'\n'字元
  }
   *
   */
  Serial.printf("檔案大小：%u位元組\n", file.size());
  Serial.println("檔案內容：");

  while (file.available()) {
    Serial.write(file.read());
  }
  
  file.close();
}

void listFile() {    // 列舉檔案
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while (file) {
    Serial.printf("檔名：%s\n", file.name());
    file = root.openNextFile();
  }

  Serial.printf("總空間：%u位元組\n", SPIFFS.totalBytes());
  Serial.printf("已使用：%u位元組\n", SPIFFS.usedBytes());
}

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("無法掛載SPIFFS檔案系統～");
    while(1)  delay(10);
  }
  
  writeFile();  // 寫入檔案
  readFile();  // 讀取檔案
  listFile();   // 列舉檔案
}

void loop() { }
