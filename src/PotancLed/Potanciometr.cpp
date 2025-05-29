#include "Potanciometr.h"

Potanciometr::Potanciometr(int pin){
    this->Pin = pin;
    pinMode(Pin, INPUT);
}

int Potanciometr::GetSignal(){
    return analogRead(Pin);
}