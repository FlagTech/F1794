// 參閱10-7頁

void setup()
{
  struct tm xmas = {30, 40, 15, 25, 11, (2025 - 1900)};
  time_t today = mktime(&xmas);
  struct tm *localtm = localtime(&today); // 定義tm結構時間
  printf("asctime輸出：%s\n", asctime(localtm));
  printf("ctime輸出：%s", ctime(&today));
}

void loop() {}
