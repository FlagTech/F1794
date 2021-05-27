// 參閱12-34頁

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

  PriceTag tag; // 宣告價格標籤（父類別）物件
  tag.price = 89;
  RedTag redTag; // 宣告紅標（子類別）物件
  redTag.price = 89;
  Serial.printf("一般含稅價格：%.1f元\n", tag.tax());
  Serial.printf("紅標含稅價格：%.1f元\n", redTag.tax());
}

void loop() {}
