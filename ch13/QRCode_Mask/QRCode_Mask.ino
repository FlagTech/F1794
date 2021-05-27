// 參閱13-17頁

void setup()
{
    Serial.begin(115200);
    Serial.println("\n");

    for (byte i = 0; i < 7; i++)
    {
        for (byte j = 0; j < 16; j++)
        {
            if ((i * j) % 2 + (i * j) % 3 == 0)
            {
                Serial.print("\u25A0");
            }
            else
            {
                Serial.print("\u25A1");
            }
        }
        Serial.print("\n");
    }
}

void loop()
{
}
