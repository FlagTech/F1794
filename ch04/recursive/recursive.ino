// 參閱4-30頁

long fact(int n)
{
  if (n == 0)
    return 1;
  else
    return n * fact(n - 1);
}

void setup()
{
  Serial.begin(115200);
  long ans = fact(4); // 計算4的階層
  Serial.print(ans);
}

void loop() {}
