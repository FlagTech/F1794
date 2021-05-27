// 參閱4-34頁

void setup()
{
  Serial.begin(115200);
  Serial.printf("HEAP可用大小：\t%u\n", ESP.getFreeHeap());

  int *pt = (int *)malloc(100 * sizeof(int)); // 在堆積區建立陣列
  for (byte i = 0; i < 100; i++)
  {
    pt[i] = i;
  }

  Serial.printf("動態建立陣列之後：\t%u\n", ESP.getFreeHeap());
  free(pt); // 刪除記憶體
  Serial.printf("刪除陣列之後：\t%u\n", ESP.getFreeHeap());
}

void loop() {}
