#include <Arduino.h>
#include"PotancLed/Potanciometr.h"
#include"PotancLed/RGBLed.h"
#include "BluetoothSerial.h"

class BlueToothWorker{
private:
    RGBLed* led;
    Potanciometr* p;
    BluetoothSerial* SerialBT;
public:
    BlueToothWorker(int SignalPin,int redPin,int greenPin, int bluePin,BluetoothSerial* bt);
    void Work(String msg, String startStr);
};