#include "BlueToothWorker.h"

    BlueToothWorker::BlueToothWorker(int SignalPin,int redPin,int greenPin, int bluePin,BluetoothSerial* bt){
        led = new RGBLed(redPin,greenPin,bluePin);
        p = new Potanciometr(SignalPin);
        SerialBT = bt;
    }


    void BlueToothWorker::Work(String msg, String startStr){

    String fromModul = startStr;

    if(msg.equals("red\r\n")){
      led->SetRed();
      fromModul.concat("set red\n");
      SerialBT->print(fromModul);
    Serial.print(fromModul);
    }
    else if(msg.equals("green\r\n")){
      led->SetGreen();
      fromModul.concat("set green\n");
      SerialBT->print(fromModul);
    Serial.print(fromModul);
    }
    else if(msg.equals("blue\r\n")){
      led->SetBlue();
      fromModul.concat("set blue\n");
      SerialBT->print(fromModul);
    Serial.print(fromModul);
    }
    else if(msg.equals("get\r\n")){
        fromModul.concat("now signal is "  + String(p->GetSignal()) + "\n");
        SerialBT->print(fromModul);
        Serial.print(fromModul);
      
    }
    else{
      fromModul.concat("wrong input: " + msg + "\n");
        SerialBT->print(fromModul);
        Serial.print(fromModul);
    }
    }