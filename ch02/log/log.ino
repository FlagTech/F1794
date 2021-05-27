// 參閱2-8頁
void setup()
{
  int counter = 10;
  log_d("counter值：%u", (counter + 10));
  log_e("出錯了～沒有初始化序列埠。");
  log_w("序列埠速率要設成 %u", 115200);
}

void loop() {}
