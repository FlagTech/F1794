#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

const int LED = LED_BUILTIN; // 定義LED接腳，設成板子內建的LED。
bool bleConnected = false;
BLECharacteristic *pCharact_TX;

class ServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    bleConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  { // 斷線回呼
    bleConnected = false;
    Serial.println("連線中斷");
    BLEDevice::startAdvertising(); // 重新發出廣告
  }
};

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  BLEDevice::init("ESP32藍牙LED開關");                         // 建立BLE裝置
  BLEServer *pServer = BLEDevice::createServer();              // 建立BLE伺服器
  pServer->setCallbacks(new ServerCallbacks());                // 設定BLE伺服器的回呼
  BLEService *pService = pServer->createService(SERVICE_UUID); // 建立BLE服務
  pCharact_TX = pService->createCharacteristic(                // 定義TX特徵物件的內容
      CHARACTERISTIC_UUID_TX,                                  // TX特徵的UUID
      BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_READ);
  pCharact_TX->addDescriptor(new BLE2902()); // 新增描述

  pService->start();                  // 啟動服務
  pServer->getAdvertising()->start(); // 開始廣播
  Serial.println("等待用戶端連線…");
}

void loop()
{
  if (bleConnected)
  {
    int hallVal = hallRead(); // 讀取霍爾感測器值
    char buffer[5];
    itoa(hallVal, buffer, 10);
    pCharact_TX->setValue(buffer);

    pCharact_TX->notify();
    Serial.printf("送出通知：%d\n", hallVal);
  }
  delay(500);
}
