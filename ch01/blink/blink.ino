#define BLINK_GPIO (gpio_num_t)5

void setup() {
#if ESP_ARDUINO_VERSION <  ESP_ARDUINO_VERSION_VAL(2, 0, 0)
  gpio_pad_select_gpio(BLINK_GPIO);  // 選定要控制的接腳
#else
  gpio_reset_pin(BLINK_GPIO);        // 重置控制接腳
#endif
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

void loop() {
  Serial.begin(115200);
  printf("輸出高電位\n");
  gpio_set_level(BLINK_GPIO, 0);          // 輸出低電位
  vTaskDelay(1000 / portTICK_PERIOD_MS);  // 延遲1000毫秒
  printf("輸出低電位\n");
  gpio_set_level(BLINK_GPIO, 1);          // 輸出高電位
  vTaskDelay(1000 / portTICK_PERIOD_MS);  // 延遲1000毫秒
}
