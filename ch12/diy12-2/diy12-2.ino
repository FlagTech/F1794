#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "switch.h"

#define BITS 10
#define STEPS 20

const byte LED = 22;
const byte SW_UP = 19;  // 「上」按鍵
const byte SW_DW = 18;  // 「下」按鍵
const byte CHANG_VAL = 1024 / STEPS;
const char* CONFIG_FILE = "/config.json";
const uint16_t SAVE_INTERVAL = 5000;  // 存檔延遲時間
bool clicked = false;   // 按鍵是否被「按一下」
uint32_t clickTime = 0; // 按鍵被按一下的時間
uint16_t pwmVal = 0;    // 電源輸出值

Switch upSW(SW_UP, LOW, true);
Switch downSW(SW_DW, LOW, true);

void lightUp() {
  if ((pwmVal + CHANG_VAL) <= 1023) {
    pwmVal += CHANG_VAL;
    Serial.println(pwmVal);
    ledcWrite(0, pwmVal);
  }
}

void lightDown() {
  if ((pwmVal - CHANG_VAL) >= 0) {
    pwmVal -= CHANG_VAL;
    Serial.println(pwmVal);
    ledcWrite(0, pwmVal);
  }
}

void writeJson(uint16_t n) {
  StaticJsonDocument<100> doc;
  doc["light"] = n;
  doc["desc"] = "紀錄亮度";

  String jsonStr;
  serializeJson(doc, jsonStr);
  Serial.printf("JSON字串：%s\n", jsonStr.c_str());

  File file = SPIFFS.open(CONFIG_FILE, "w");

  if (!file) {
    Serial.println("無法開啟檔案～");
    return;
  } else {
    file.println(jsonStr.c_str());
    file.close();
  }
}

uint16_t readJson() {
  String jsonStr;
  File file = SPIFFS.open(CONFIG_FILE, "r");

  if (!file || file.size() == 0) {
    Serial.println("無法開啟檔案～");
    return 0;
  } else {
    while (file.available()) {
      jsonStr = file.readStringUntil('\n');
    }
    Serial.printf("檔案內容：%s\n", jsonStr.c_str());
    file.close();
  }
  StaticJsonDocument<100> doc;

  deserializeJson(doc, jsonStr);
  uint16_t light = doc["light"];
  String desc = doc["desc"];

  Serial.printf("light值：%u、desc值：%s\n", light, desc.c_str());
  return light;
}


void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);
  ledcSetup(0, 5000, BITS);
  ledcAttachPin(LED, 0);

  if (!SPIFFS.begin()) {
    Serial.println("無法掛載SPIFFS檔案系統～");
    while (1) {
      delay(10);
    }
  }

  // 讀取存檔紀錄
  pwmVal = readJson();
  ledcWrite(0, pwmVal);
}

void loop() {
  // "上"按鍵
  switch (upSW.check()) {
    case RELEASED_FROM_PRESS:
    case PRESSING:
      clicked = true;
      clickTime = millis();
      lightUp();
      break;
  }

  // "下"按鍵
  switch (downSW.check()) {
    case RELEASED_FROM_PRESS:
    case PRESSING:
      clicked = true;
      clickTime = millis();
      lightDown();
      break;
  }

  if (clicked && (millis()-clickTime>=SAVE_INTERVAL)) {
    // 延遲5秒後紀錄
    clicked = false;
    writeJson(pwmVal);
  }
}
