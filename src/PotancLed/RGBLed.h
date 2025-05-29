#include <Arduino.h>

class RGBLed
{
private:
    int rPin;
    int gPin;
    int bPin;
public:
    RGBLed(int rPin,int gPin,int bPin);
    void SetRed();
    void SetBlue();
    void SetGreen();
    void SetColor(uint8_t r,uint8_t g, uint8_t b);
    ~RGBLed();
};