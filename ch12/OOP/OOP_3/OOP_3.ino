// 參閱12-36頁

class PriceTag
{
public:
  float price = 0;

  float tax()
  {
    return price * 1.05;
  };
};

class RedTag : public PriceTag
{
public:
  float tax()
  {
    return price;
  };
};

void setup()
{
  Serial.begin(115200);
  PriceTag *pt;
  pt = new PriceTag();
  pt->price = 89;
  Serial.printf("一般含稅價格：%.1f元\n", pt->tax());
  pt = new RedTag();
  pt->price = 89;
  Serial.printf("紅標含稅價格：%.1f元\n", pt->tax());
}

void loop() {}
