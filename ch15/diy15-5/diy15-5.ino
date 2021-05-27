#include <Wire.h> 
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219(0x40);

void setup(void) {
  Serial.begin(115200);
  
  if (! ina219.begin()) {
    Serial.printf("找不到INA219晶片～\n");
    while (1) delay(10);
  }

  Serial.println("開始用INA219測量電壓和電流...\n");
}

void loop(void) {
  float shuntvoltage = ina219.getShuntVoltage_mV(); // 讀取分流電壓
  float busvoltage = ina219.getBusVoltage_V();  // 匯流排電壓
  float current_mA = ina219.getCurrent_mA();  // 電流
  float power_mW = ina219.getPower_mW();  // 消耗功率
  float loadvoltage = busvoltage + (shuntvoltage / 1000);

  // 所有浮點數值都取到小數點後2位，輸出到序列埠。
  Serial.printf("匯流排電壓：%.2f V\n", busvoltage);
  Serial.printf("分流電壓：%.2f mV\n", shuntvoltage);
  Serial.printf("電源電壓：%.2f V\n", loadvoltage);
  Serial.printf("電流：%.2f mA\n", current_mA);
  Serial.printf("功耗：%.2f mW\n", power_mW);
  Serial.println("==========================");
 
  delay(2000);
}
