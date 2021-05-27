#include <esp_bt_device.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID           "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID_RX "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID_TX "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

const int LED = 22;  // LED接腳
bool deviceConnected = false;
BLECharacteristic *pCharact_TX;

class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("連線中斷");
      BLEDevice::startAdvertising(); // 重新發出廣告
    }
};

class RXCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharact) {
      std::string rxVal = pCharact->getValue();
      Serial.printf("收到輸入值：%s\n", rxVal.c_str());

      if (rxVal == "on") {
        Serial.println("開燈！");
        digitalWrite(LED, LOW);
      } else if (rxVal == "off") {
        Serial.println("關燈！");
        digitalWrite(LED, HIGH);
      }
    }
};

class TXCallbacks: public BLECharacteristicCallbacks {
    void onStatus(BLECharacteristic *pCharact, Status s, uint32_t code) {
      Serial.printf("狀態碼：%d\n", s);
      Serial.printf("code：%d\n", code);
    }
};

void printDeviceAddress() {
  const uint8_t* point = esp_bt_dev_get_address();
  for (int i = 0; i < 6; i++) {
    char str[3];
    sprintf(str, "%02X", (int)point[i]);
    Serial.print(str);
 
    if (i < 5){
      Serial.print(":");
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  BLEDevice::init("ESP32藍牙LED開關");

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  // 建立服務
  BLEService *pService = pServer->createService(SERVICE_UUID);
  // 建立特徵
  pCharact_TX = pService->createCharacteristic(
                  CHARACTERISTIC_UUID_TX,
                  BLECharacteristic::PROPERTY_NOTIFY |
                  BLECharacteristic::PROPERTY_READ
                );

  pCharact_TX->addDescriptor(new BLE2902());
  pCharact_TX->setCallbacks(new TXCallbacks());
  

  BLECharacteristic *pCharact_RX = pService->createCharacteristic(
                                     CHARACTERISTIC_UUID_RX,
                                     BLECharacteristic::PROPERTY_WRITE
                                   );

  pCharact_RX->setCallbacks(new RXCallbacks());

  BLEDescriptor *pDesc = new BLEDescriptor((uint16_t)0x2901);
  pDesc->setValue("控制板內建LED的開關");
  pCharact_RX->addDescriptor(pDesc);
  // 啟動服務
  pService->start();
  // 開始廣播
  pServer->getAdvertising()->start();
  Serial.println("等待用戶端連線…");
  printDeviceAddress();
}

void loop() {
  if (deviceConnected) {
    int hallVal = hallRead();
    char buffer[5];
    itoa(hallVal, buffer, 10);
    pCharact_TX->setValue(buffer);

    pCharact_TX->notify();
    Serial.printf("送出： %d\n", hallVal);
  }
  delay(500);
}
