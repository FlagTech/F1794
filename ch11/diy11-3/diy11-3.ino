#include <driver/rtc_io.h>
#define SLEEP_SEC 3          // 睡眠3秒

RTC_DATA_ATTR bool LEDstate = 0;  // 紀錄LED狀態
gpio_num_t LED = GPIO_NUM_0;   // 定義LED接腳

void setup() {
  Serial.begin(115200);
  rtc_gpio_init(LED);
  rtc_gpio_set_direction(LED, RTC_GPIO_MODE_OUTPUT_ONLY);
  LEDstate = !LEDstate;           // 反相布林值
  rtc_gpio_set_level(LED, LEDstate); // 在LED接腳輸出數位訊號
  Serial.println("我醒著～");
  delay(100);
  // 讓RTC GPIO和ULP在睡眠時持續運作
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
  esp_sleep_enable_timer_wakeup(SLEEP_SEC * 1000000L);  
  Serial.println("睡覺了～");
  delay(100);
  esp_deep_sleep_start();  // 進入深度睡眠
}

void loop() { }
