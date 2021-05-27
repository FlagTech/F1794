// 參閱4-33頁

void setup()
{
  Serial.begin(115200);
  Serial.printf("HEAP可用大小：\t%u\n", ESP.getFreeHeap());

  int *pt = new int[100]; // 在堆積區建立陣列

  for (byte i = 0; i < 100; i++)
  {
    pt[i] = i; // 分別存入0~99
  }

  Serial.printf("動態建立陣列之後：\t%u\n", ESP.getFreeHeap());
  delete pt; // 刪除記憶體
  Serial.printf("刪除陣列之後：\t%u\n", ESP.getFreeHeap());
}

void loop() {}
