#include <Arduino.h>

class Potanciometr{
private:
    int Pin;
public:
    Potanciometr(int Pin);
    int GetSignal();
};