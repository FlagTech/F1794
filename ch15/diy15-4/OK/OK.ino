#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define UART_SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define BATT_SERVICE_UUID (uint16_t)0x180F // 建立新的servive
#define BITS 10
#define BATT_SENSOR_PIN A0

const uint16_t ADC_RES = 1023; // 10位元解析度

float voltData[21] = {
  3.0, 3.5, 3.58, 3.64, 3.67,
  3.7, 3.71, 3.72, 3.74, 3.75,
  3.76, 3.78, 3.81, 3.84, 3.86,
  3.91, 3.95, 3.98, 4.04, 4.1, 4.2
};

const int LED = 22;
bool deviceConnected = false;
BLECharacteristic *pCharactTX;
BLECharacteristic *pCharactBatt;  // 電量的特質

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer) {
      deviceConnected = false;
    }
};

class RXCallbacks : public BLECharacteristicCallbacks {
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

class TXCallbacks : public BLECharacteristicCallbacks {
    void onStatus(BLECharacteristic *pCharact, Status s, uint32_t code) {
      Serial.printf("狀態碼：%d\n", s);
      Serial.printf("code：%d\n", code);
    }
};

uint8_t checkBatt() {
  uint16_t adc = analogRead(BATT_SENSOR_PIN);
  float volt = adc * 0.00411;
  uint8_t battLevel = 0;

  // 大於700以上才需要比對電位
  if (adc > 700) {
    for (int8_t i = 20; i >= 0; i--) {
      if (volt >= voltData[i]) {
        battLevel = i * 5;
        break;
      }
    }
  }

  return battLevel;  // 電量百分比
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);

  BLEDevice::init("ESP32藍牙LED開關");

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  // 建立服務
  BLEService* pUARTService = pServer->createService(UART_SERVICE_UUID);
  // 建立特徵
  pCharactTX = pUARTService->createCharacteristic(
                 CHARACTERISTIC_UUID_TX,
                 BLECharacteristic::PROPERTY_NOTIFY |
                 BLECharacteristic::PROPERTY_READ);

  pCharactTX->addDescriptor(new BLE2902());
  pCharactTX->setCallbacks(new TXCallbacks());

  BLECharacteristic* pCharactRX = pUARTService->createCharacteristic(
                                    CHARACTERISTIC_UUID_RX,
                                    BLECharacteristic::PROPERTY_WRITE);

  pCharactRX->setCallbacks(new RXCallbacks());

  BLEDescriptor* pDesc = new BLEDescriptor((uint16_t)0x2901);
  pDesc->setValue("控制板內建LED的開關");
  pCharactRX->addDescriptor(pDesc);

  // 新建電池的BLE服務
  BLEService* pBattService = pServer->createService(BATT_SERVICE_UUID);

  pCharactBatt = pBattService->createCharacteristic((uint16_t)0x2A19,
                 BLECharacteristic::PROPERTY_READ |
                 BLECharacteristic::PROPERTY_NOTIFY);

  BLEDescriptor* pBattDesc = new BLEDescriptor((uint16_t)0x2901);
  pBattDesc->setValue("電量0~100");
  pCharactBatt->addDescriptor(pBattDesc);
  pCharactBatt->addDescriptor(new BLE2902());
  // 啟動服務
  pUARTService->start();
  pBattService->start();
  // 開始廣播
  pServer->getAdvertising()->start();
  Serial.println("等待用戶端連線…");
}

void loop() {
  if (deviceConnected) {
    uint8_t battLevel = checkBatt();
    int hallVal = hallRead();
    char buffer[5];
    itoa(hallVal, buffer, 10);
    pCharactTX->setValue(buffer);

    pCharactTX->notify();
    pCharactBatt->setValue(&battLevel, 1);
    pCharactBatt->notify();
    delay(1000);
  }
}
