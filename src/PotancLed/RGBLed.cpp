#include <Arduino.h>
#include "RGBLed.h"

RGBLed::RGBLed(int rPin,int gPin,int bPin)
{
    this->rPin = rPin;
    this->bPin = bPin;
    this->gPin = gPin;
    pinMode(rPin, OUTPUT);
    pinMode(gPin, OUTPUT);
    pinMode(bPin, OUTPUT);
    digitalWrite(rPin, HIGH);
    digitalWrite(gPin, HIGH);
    digitalWrite(bPin, HIGH);
}

void RGBLed::SetRed()
{
    digitalWrite(rPin, LOW);
    digitalWrite(gPin, HIGH);
    digitalWrite(bPin, HIGH);
}

void RGBLed::SetBlue()
{
    digitalWrite(rPin, HIGH);
    digitalWrite(gPin, HIGH);
    digitalWrite(bPin, LOW);
}

void RGBLed::SetGreen()
{
    digitalWrite(rPin, HIGH);
    digitalWrite(gPin, LOW);
    digitalWrite(bPin, HIGH);
}

void RGBLed::SetColor(uint8_t r,uint8_t g, uint8_t b){
    digitalWrite(rPin, r);
    digitalWrite(gPin, g);
    digitalWrite(bPin, b);
}

RGBLed::~RGBLed()
{
}


